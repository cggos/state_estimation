#include <iostream>
#include <stdint.h>
#include <unordered_set>
#include <memory>
#include <vector>
#include <stdlib.h>

#include <Eigen/StdVector>
#include <Eigen/Core>

#include "g2o/stuff/sampler.h"
#include "g2o/core/base_vertex.h"
#include "g2o/core/base_binary_edge.h"
#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/block_solver.h"
#include "g2o/core/solver.h"
#include "g2o/core/robust_kernel_impl.h"
#include "g2o/core/batch_stats.h"
#include "g2o/core/optimization_algorithm_levenberg.h"
#include "g2o/core/optimization_algorithm_dogleg.h"

#include "g2o/solvers/cholmod/linear_solver_cholmod.h"
#include "g2o/solvers/dense/linear_solver_dense.h"
#include "g2o/solvers/eigen/linear_solver_eigen.h"
#include "g2o/solvers/pcg/linear_solver_pcg.h"
#include "g2o/types/sba/types_six_dof_expmap.h"

#include "g2o/solvers/structure_only/structure_only_solver.h"

#include <ceres/internal/autodiff.h>

#include "common/BundleParams.h"
#include "common/BALProblem.h"
#include "common/projection.h"

using namespace Eigen;
using namespace std;

typedef Eigen::Map<Eigen::VectorXd> VectorRef;
typedef Eigen::Map<const Eigen::VectorXd> ConstVectorRef;
typedef g2o::BlockSolver<g2o::BlockSolverTraits<9,3> > BalBlockSolver;

class VertexCameraBAL : public g2o::BaseVertex<9,Eigen::VectorXd> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    VertexCameraBAL() {}

    virtual bool read(std::istream & /*is*/ ) { return false; }

    virtual bool write(std::ostream & /*os*/ ) const { return false; }

    virtual void setToOriginImpl() {}

    virtual void oplusImpl(const double *update) {
        Eigen::VectorXd::ConstMapType v(update, VertexCameraBAL::Dimension);
        _estimate += v;
    }
};

class VertexPointBAL : public g2o::BaseVertex<3, Eigen::Vector3d> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    VertexPointBAL() {}

    virtual bool read(std::istream & /*is*/ ) { return false; }

    virtual bool write(std::ostream & /*os*/ ) const { return false; }

    virtual void setToOriginImpl() {}

    virtual void oplusImpl(const double *update) {
        Eigen::Vector3d::ConstMapType v(update);
        _estimate += v;
    }
};

class EdgeObservationBAL : public g2o::BaseBinaryEdge<2, Eigen::Vector2d,VertexCameraBAL, VertexPointBAL> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    EdgeObservationBAL() {}

    virtual bool read(std::istream & /*is*/ ) { return false; }

    virtual bool write(std::ostream & /*os*/ ) const { return false; }

    // The virtual function comes from the Edge base class. Must define if you use edge.
    virtual void computeError() override {
        const VertexCameraBAL *cam = static_cast<const VertexCameraBAL *> ( vertex(0));
        const VertexPointBAL *point = static_cast<const VertexPointBAL *> ( vertex(1));
        (*this)(cam->estimate().data(), point->estimate().data(), _error.data());
    }

    template<typename T>
    bool operator()(const T *camera, const T *point, T *residuals) const {
        T predictions[2];
        CamProjectionWithDistortion(camera, point, predictions);
        residuals[0] = predictions[0] - T(measurement()(0));
        residuals[1] = predictions[1] - T(measurement()(1));
        return true;
    }

    virtual void linearizeOplus() override {
        const VertexCameraBAL *cam  = static_cast<const VertexCameraBAL*> (vertex(0));
        const VertexPointBAL *point = static_cast<const VertexPointBAL *> (vertex(1));

        Eigen::Matrix<double, Dimension, VertexCameraBAL::Dimension, Eigen::RowMajor> dError_dCamera;
        Eigen::Matrix<double, Dimension, VertexPointBAL::Dimension, Eigen::RowMajor> dError_dPoint;
        double *parameters[] = {const_cast<double *> ( cam->estimate().data()),
                                const_cast<double *> ( point->estimate().data())};

        double *jacobians[] = {dError_dCamera.data(), dError_dPoint.data()};
        double value[Dimension];
        bool diffState = // using autodiff from ceres. Otherwise, the system will use g2o numerical diff for Jacobians
                ceres::internal::AutoDiff<EdgeObservationBAL, double, VertexCameraBAL::Dimension, VertexPointBAL::Dimension>::
                Differentiate(*this, parameters, Dimension, value, jacobians);

        // copy over the Jacobians (convert row-major -> column-major)
        if (diffState) {
            _jacobianOplusXi = dError_dCamera; // 2x9
            _jacobianOplusXj = dError_dPoint;  // 2x3
        } else {
            assert (0 && "Error while differentiating");
            _jacobianOplusXi.setZero();
            _jacobianOplusXi.setZero();
        }
    }
};

// set up the vertexs and edges for the bundle adjustment.
void BuildProblem(const BALProblem* bal_problem, g2o::SparseOptimizer* optimizer, const BundleParams& params) {
    const int num_points        = bal_problem->num_points();
    const int num_cameras       = bal_problem->num_cameras();
    const int camera_block_size = bal_problem->camera_block_size();
    const int point_block_size  = bal_problem->point_block_size();
    const int num_observations  = bal_problem->num_observations();

    // Set camera vertex with initial value in the dataset.
    const double* raw_cameras = bal_problem->cameras();
    for(int i = 0; i < num_cameras; ++i) {
        ConstVectorRef temVecCamera(raw_cameras + camera_block_size * i, camera_block_size);
        VertexCameraBAL *pCamera = new VertexCameraBAL();
        pCamera->setEstimate(temVecCamera);   // initial value for the camera i..
        pCamera->setId(i);                    // set id for each camera vertex
        optimizer->addVertex(pCamera);
    }

    // Set point vertex with initial value in the dataset.
    const double* raw_points = bal_problem->points();
    for(int j = 0; j < num_points; ++j) {
        ConstVectorRef temVecPoint(raw_points + point_block_size * j, point_block_size);
        VertexPointBAL *pPoint = new VertexPointBAL();
        pPoint->setEstimate(temVecPoint);   // initial value for the point i..
        // each vertex should have an unique id, no matter it is a camera vertex, or a point vertex
        pPoint->setId(j + num_cameras);
        pPoint->setMarginalized(true);
        optimizer->addVertex(pPoint);
    }

    // Set edges for graph..
    const double* observations = bal_problem->observations();   // pointer for the first observation..
    for(int i = 0; i < num_observations; ++i) {
        EdgeObservationBAL *bal_edge = new EdgeObservationBAL();

        if (params.robustify) {
            g2o::RobustKernelHuber *rk = new g2o::RobustKernelHuber;
            rk->setDelta(1.0);
            bal_edge->setRobustKernel(rk);
        }

        const int camera_id = bal_problem->camera_index()[i];              // get id for the camera;
        const int point_id  = bal_problem->point_index()[i] + num_cameras; // get id for the point

        // set the vertex by the ids for an edge observation
        bal_edge->setVertex(0, dynamic_cast<VertexCameraBAL *>(optimizer->vertex(camera_id)));
        bal_edge->setVertex(1, dynamic_cast<VertexPointBAL *>(optimizer->vertex(point_id)));
        bal_edge->setInformation(Eigen::Matrix2d::Identity());
        bal_edge->setMeasurement(Eigen::Vector2d(observations[2 * i + 0], observations[2 * i + 1]));

        optimizer->addEdge(bal_edge);
    }
}

void WriteToBALProblem(BALProblem* bal_problem, g2o::SparseOptimizer* optimizer) {
    const int num_points  = bal_problem->num_points();
    const int num_cameras = bal_problem->num_cameras();
    const int camera_block_size = bal_problem->camera_block_size();
    const int point_block_size  = bal_problem->point_block_size();

    double *raw_cameras = bal_problem->mutable_cameras();
    for (int i = 0; i < num_cameras; ++i) {
        VertexCameraBAL *pCamera = dynamic_cast<VertexCameraBAL *>(optimizer->vertex(i));
        Eigen::VectorXd NewCameraVec = pCamera->estimate();
        memcpy(raw_cameras + i * camera_block_size, NewCameraVec.data(), sizeof(double) * camera_block_size);
    }

    double *raw_points = bal_problem->mutable_points();
    for (int j = 0; j < num_points; ++j) {
        VertexPointBAL *pPoint = dynamic_cast<VertexPointBAL *>(optimizer->vertex(j + num_cameras));
        Eigen::Vector3d NewPointVec = pPoint->estimate();
        memcpy(raw_points + j * point_block_size, NewPointVec.data(), sizeof(double) * point_block_size);
    }
}

void SetSolverOptionsFromFlags(BALProblem* bal_problem, const BundleParams& params, g2o::SparseOptimizer* optimizer) {
    g2o::LinearSolver<BalBlockSolver::PoseMatrixType> *linearSolver = 0;
    if (params.linear_solver == "dense_schur") {
        linearSolver = new g2o::LinearSolverDense<BalBlockSolver::PoseMatrixType>();
    } else if (params.linear_solver == "sparse_schur") {
        linearSolver = new g2o::LinearSolverCholmod<BalBlockSolver::PoseMatrixType>();
        // AMD ordering , only needed for sparse cholesky solver
        dynamic_cast<g2o::LinearSolverCholmod<BalBlockSolver::PoseMatrixType> * >(linearSolver)->setBlockOrdering(true);
    }

    BalBlockSolver *solver_ptr = new BalBlockSolver(linearSolver);
    g2o::OptimizationAlgorithmWithHessian *solver;
    if (params.trust_region_strategy == "levenberg_marquardt") {
        solver = new g2o::OptimizationAlgorithmLevenberg(solver_ptr);
    } else if (params.trust_region_strategy == "dogleg") {
        solver = new g2o::OptimizationAlgorithmDogleg(solver_ptr);
    } else {
        std::cout << "Please check your trust_region_strategy parameter again.." << std::endl;
        exit(EXIT_FAILURE);
    }
    optimizer->setAlgorithm(solver);
}

void SolveProblem(const char* filename, const BundleParams& params) {
    BALProblem bal_problem(filename);

    std::cout << "\n\n"
              << "bal problem file loaded..." << std::endl
              << "bal problem have " << bal_problem.num_cameras() << " cameras and "
              << bal_problem.num_points() << " points. " << std::endl
              << "Forming " << bal_problem.num_observations() << " observatoins. " << std::endl;

    // store the initial 3D cloud points and camera pose..
    if (!params.initial_ply.empty()) {
        bal_problem.WriteToPLYFile(params.initial_ply);
    }

    // add some noise for the intial value
    srand(params.random_seed);
    bal_problem.Normalize();
    bal_problem.Perturb(params.rotation_sigma, params.translation_sigma, params.point_sigma);

    {
        g2o::SparseOptimizer optimizer;
        SetSolverOptionsFromFlags(&bal_problem, params, &optimizer);
        BuildProblem(&bal_problem, &optimizer, params);
        optimizer.initializeOptimization();
        optimizer.setVerbose(true);
        optimizer.optimize(params.num_iterations);
        // write the optimized data into BALProblem class
        WriteToBALProblem(&bal_problem, &optimizer);
    }

    // write the result into a .ply file.
    if (!params.final_ply.empty()) {
        bal_problem.WriteToPLYFile(params.final_ply);
    }
}

int main(int argc, char** argv)
{
    BundleParams params(argc, argv);

    if(params.input.empty()){
        std::cout << "Usage: ba_g2o -input <path to bal_dataset: problem-*.txt>\n";
        return 1;
    }

    SolveProblem(params.input.c_str(), params);

    return 0;
}
