#ifndef G2O_IMP_H
#define G2O_IMP_H

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

#include "bal_projection.h"

class VertexCamera : public g2o::BaseVertex<9,Eigen::VectorXd> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    VertexCamera() {}

    virtual bool read(std::istream & /*is*/ ) { return false; }

    virtual bool write(std::ostream & /*os*/ ) const { return false; }

    virtual void setToOriginImpl() {}

    virtual void oplusImpl(const double *update) {
        Eigen::VectorXd::ConstMapType v(update, VertexCamera::Dimension);
        _estimate += v;
    }
};

class VertexPoint : public g2o::BaseVertex<3, Eigen::Vector3d> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    VertexPoint() {}

    virtual bool read(std::istream & /*is*/ ) { return false; }

    virtual bool write(std::ostream & /*os*/ ) const { return false; }

    virtual void setToOriginImpl() {}

    virtual void oplusImpl(const double *update) {
        Eigen::Vector3d::ConstMapType v(update);
        _estimate += v;
    }
};

class EdgeObservation : public g2o::BaseBinaryEdge<2, Eigen::Vector2d, VertexCamera, VertexPoint> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    EdgeObservation() {}

    virtual bool read(std::istream & /*is*/ ) { return false; }

    virtual bool write(std::ostream & /*os*/ ) const { return false; }

    // The virtual function comes from the Edge base class. Must define if you use edge.
    virtual void computeError() override {
        const VertexCamera *cam  = static_cast<const VertexCamera*>(vertex(0));
        const VertexPoint *point = static_cast<const VertexPoint*> (vertex(1));
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
        const VertexCamera *cam  = static_cast<const VertexCamera*> (vertex(0));
        const VertexPoint *point = static_cast<const VertexPoint *> (vertex(1));

        Eigen::Matrix<double, Dimension, VertexCamera::Dimension, Eigen::RowMajor> dError_dCamera;
        Eigen::Matrix<double, Dimension, VertexPoint::Dimension, Eigen::RowMajor> dError_dPoint;
        double *parameters[] = {const_cast<double *>(cam->estimate().data()), const_cast<double *>(point->estimate().data())};
        double *jacobians[]  = {dError_dCamera.data(), dError_dPoint.data()};
        
        double value[Dimension];
        bool diffState = // using autodiff from ceres. Otherwise, the system will use g2o numerical diff for Jacobians
                ceres::internal::AutoDiff<EdgeObservation, double, VertexCamera::Dimension, VertexPoint::Dimension>::
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

void SetSolverOptions(g2o::SparseOptimizer* optimizer, const std::string& param_linear_solver, const std::string& param_trust_region_strategy) {
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<9,3> > BalBlockSolver;

    // g2o::LinearSolver<BalBlockSolver::PoseMatrixType> *linearSolver = 0;
    BalBlockSolver::LinearSolverType *linearSolver = 0;
    if (param_linear_solver == "dense_schur") {
        linearSolver = new g2o::LinearSolverDense<BalBlockSolver::PoseMatrixType>();
    } else if (param_linear_solver == "sparse_schur") {
        linearSolver = new g2o::LinearSolverCholmod<BalBlockSolver::PoseMatrixType>();
        // AMD ordering , only needed for sparse cholesky solver
        dynamic_cast<g2o::LinearSolverCholmod<BalBlockSolver::PoseMatrixType> * >(linearSolver)->setBlockOrdering(true);
    }

    BalBlockSolver *solver_ptr = new BalBlockSolver(std::unique_ptr<BalBlockSolver::LinearSolverType>(linearSolver));

    g2o::OptimizationAlgorithmWithHessian *solver;
    if (param_trust_region_strategy == "levenberg_marquardt") {
        solver = new g2o::OptimizationAlgorithmLevenberg(std::unique_ptr<BalBlockSolver>(solver_ptr));
    } else if (param_trust_region_strategy == "dogleg") {
        solver = new g2o::OptimizationAlgorithmDogleg(std::unique_ptr<BalBlockSolver>(solver_ptr));
    } else {
        std::cout << "Please check your trust_region_strategy parameter again.." << std::endl;
        exit(EXIT_FAILURE);
    }

    optimizer->setAlgorithm(solver);
}

#endif // G2O_IMP_H