#include <iostream>
#include <fstream>

#include <sophus/se3.h>

#include <g2o/stuff/sampler.h>
#include <g2o/core/base_vertex.h>
#include <g2o/core/base_binary_edge.h>
#include <g2o/core/sparse_optimizer.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/solver.h>
#include <g2o/core/robust_kernel_impl.h>
#include <g2o/core/batch_stats.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_dogleg.h>

#include <g2o/solvers/cholmod/linear_solver_cholmod.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>
#include <g2o/solvers/pcg/linear_solver_pcg.h>
#include <g2o/types/sba/types_six_dof_expmap.h>

#include <g2o/solvers/structure_only/structure_only_solver.h>

typedef std::vector<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> VecVector3d;
typedef std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> VecVector2d;

void SetSolverOptions(g2o::SparseOptimizer* optimizer, const std::string& param_linear_solver, const std::string& param_trust_region_strategy) {
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<6, 3> > BalBlockSolver;

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

int main(int argc, char **argv)
{
    int n_points = 0;

    VecVector2d p2d;
    VecVector3d p3d;
    {
        std::string p2d_file = "../../../bundle_adjustment/ba_gauss_newton/data/p2d.txt";
        std::string p3d_file = "../../../bundle_adjustment/ba_gauss_newton/data/p3d.txt";

        std::ifstream fileP2D(p2d_file);
        std::ifstream fileP3D(p3d_file);

        if (!fileP2D.is_open() || !fileP3D.is_open()) {
            std::cout << "fileP2D or fileP3D open FAILED!" << std::endl;
            return -1;
        }

        double u = 0, v = 0;
        double x = 0, y = 0, z = 0;
        while (!fileP2D.eof()) {
            if (fileP2D.fail())
                break;
            fileP2D >> u >> v;
            fileP3D >> x >> y >> z;
            p2d.push_back(Eigen::Vector2d(u, v));
            p3d.push_back(Eigen::Vector3d(x, y, z));
        }

        if(fileP2D.is_open())
            fileP2D.close();
        if(fileP3D.is_open())
            fileP3D.close();

        assert(p3d.size() == p2d.size());

        n_points = p3d.size();
        std::cout << "points: " << n_points << std::endl;
    }

    g2o::SparseOptimizer optimizer;
    SetSolverOptions(&optimizer, "sparse_schur", "levenberg_marquardt");

    // camera vertex
    g2o::VertexSE3Expmap *vSE3 = new g2o::VertexSE3Expmap();
    vSE3->setEstimate(g2o::SE3Quat());
    vSE3->setId(0);
    vSE3->setFixed(false);
    optimizer.addVertex(vSE3);

    // point vertex
    for(int i=0; i<n_points; ++i) {
        g2o::VertexSBAPointXYZ* vPoint = new g2o::VertexSBAPointXYZ();
        vPoint->setEstimate(p3d[i]);
        vPoint->setId(i+1);
        vPoint->setMarginalized(true);
        vPoint->setFixed(true);
        optimizer.addVertex(vPoint);
    }

    // edge
    for(int i = 0; i < n_points; ++i) {
        g2o::EdgeSE3ProjectXYZOnlyPose* e = new g2o::EdgeSE3ProjectXYZOnlyPose();

        e->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex*>(optimizer.vertex(0)));
        e->setMeasurement(p2d[i]);
        e->setInformation(Eigen::Matrix2d::Identity());

        if (false) {
            g2o::RobustKernelHuber *rk = new g2o::RobustKernelHuber;
            rk->setDelta(1.0);
            e->setRobustKernel(rk);
        }

        e->fx = 520.9;
        e->fy = 521.0;
        e->cx = 325.1;
        e->cy = 249.7;

        e->Xw = p3d[i];

        optimizer.addEdge(e);
    }

    optimizer.initializeOptimization();
    optimizer.setVerbose(true);
    optimizer.optimize(100);

    g2o::SE3Quat SE3Quat = vSE3->estimate(); 
    g2o::Vector6 se3_g2o = SE3Quat.log(); // 旋转在前，平移在后

    Sophus::Vector6d se3; // 平移在前，旋转在后
    for (int i=0; i<3;i++) { 
        se3[i]   = se3_g2o[i+3]; 
        se3[i+3] = se3_g2o[i]; 
    }

    std::cout << "estimated pose: \n" << Sophus::SE3::exp(se3).matrix() << std::endl;

    return 0;
}
