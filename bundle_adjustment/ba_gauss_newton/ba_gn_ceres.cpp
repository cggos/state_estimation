//
// Created by cg on 3/14/19.
//

#include <fstream>

#include <sophus/se3.h>

#include <ceres/ceres.h>

typedef std::vector<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> VecVector3d;
typedef std::vector<Eigen::Vector2d, Eigen::aligned_allocator<Eigen::Vector2d>> VecVector2d;

class BAGNCostFunctor : public ceres::SizedCostFunction<2, 6> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    BAGNCostFunctor(Eigen::Vector2d observed_p, Eigen::Vector3d observed_P) :
            observed_p_(observed_p), observed_P_(observed_P) {}

    virtual ~BAGNCostFunctor() {}

    virtual bool Evaluate(double const* const* parameters, double *residuals, double **jacobians) const {

        Eigen::Map<const Eigen::Matrix<double,6,1>> se3(*parameters);

        Sophus::SE3 T_SE3 = Sophus::SE3::exp(se3);

        Eigen::Vector3d Pc = T_SE3 * observed_P_;

        Eigen::Matrix3d K;
        double fx = 520.9, fy = 521.0, cx = 325.1, cy = 249.7;
        K << fx, 0, cx, 0, fy, cy, 0, 0, 1;

        Eigen::Vector2d residual =  (K * Pc).hnormalized() - observed_p_;

        residuals[0] = residual[0];
        residuals[1] = residual[1];

        if(jacobians != NULL) {

            if(jacobians[0] != NULL) {

                Eigen::Map<Eigen::Matrix<double, 2, 6, Eigen::RowMajor>> J(jacobians[0]);

                double x = Pc[0];
                double y = Pc[1];
                double z = Pc[2];

                double x2 = x*x;
                double y2 = y*y;
                double z2 = z*z;

                J(0,0) =  fx/z;
                J(0,1) =  0;
                J(0,2) = -fx*x/z2;
                J(0,3) = -fx*x*y/z2;
                J(0,4) =  fx+fx*x2/z2;
                J(0,5) = -fx*y/z;
                J(1,0) =  0;
                J(1,1) =  fy/z;
                J(1,2) = -fy*y/z2;
                J(1,3) = -fy-fy*y2/z2;
                J(1,4) =  fy*x*y/z2;
                J(1,5) =  fy*x/z;
            }
        }

        return true;
    }

private:
    const Eigen::Vector2d observed_p_;
    const Eigen::Vector3d observed_P_;
};

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

    Sophus::Vector6d se3;

    ceres::Problem problem;
    for(int i=0; i<n_points; ++i) {
        ceres::CostFunction *cost_function;
        cost_function = new BAGNCostFunctor(p2d[i], p3d[i]);
        problem.AddResidualBlock(cost_function, new ceres::CauchyLoss(0.5), se3.data());
    }

    ceres::Solver::Options options;
    options.dynamic_sparsity = true;
    options.max_num_iterations = 100;
    options.sparse_linear_algebra_library_type = ceres::SUITE_SPARSE;
    options.minimizer_type = ceres::TRUST_REGION;
    options.linear_solver_type = ceres::SPARSE_NORMAL_CHOLESKY;
    options.trust_region_strategy_type = ceres::DOGLEG;
    options.minimizer_progress_to_stdout = true;
    options.dogleg_type = ceres::SUBSPACE_DOGLEG;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";

    std::cout << "estimated pose: \n" << Sophus::SE3::exp(se3).matrix() << std::endl;

    return 0;
}
