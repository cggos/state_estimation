//
// A simple example of using the Ceres minimizer.
//
// Minimize 0.5 (10 - x)^2 using jacobian matrix

#include "ceres/ceres.h"
#include "glog/logging.h"

struct CostFunctor {
    template<typename T>
    bool operator()(const T *const x, T *residual) const {
        residual[0] = 10.0 - x[0]; // f(x) = 10 - x
        return true;
    }
};

struct CostFunctorNum {
    bool operator()(const double *const x, double *residual) const {
        residual[0] = 10.0 - x[0]; // f(x) = 10 - x
        return true;
    }
};

class SimpleCostFunctor : public ceres::SizedCostFunction<1,1> {
public:
    virtual ~SimpleCostFunctor() {};

    virtual bool Evaluate(double const* const* parameters, double *residuals, double **jacobians) const {
        const double x = parameters[0][0];

        residuals[0] = 10 - x; // f(x) = 10 - x

        if(jacobians != NULL && jacobians[0] != NULL) {
            jacobians[0][0] = -1; // f'(x) = -1
        }

        return true;
    }
};

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);

    double x = 0.5;
    const double initial_x = x;

    ceres::Problem problem;

    ceres::CostFunction *cost_function; // Set up the only cost function (also known as residual)
//    cost_function = new ceres::AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor); // auto-differentiation
//    cost_function = new ceres::NumericDiffCostFunction<CostFunctorNum, ceres::CENTRAL, 1, 1>(new CostFunctorNum); //numeric differentiation
    cost_function = new SimpleCostFunctor;

    problem.AddResidualBlock(cost_function, NULL, &x); // 添加代价函数cost_function和损失函数NULL，其中x为状态量

    ceres::Solver::Options options;
    options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x << " -> " << x << "\n";

    return 0;
}