#include <iostream>
#include <fstream>

#include <ceres/ceres.h>

#include "common/bal_problem.h"
#include "common/bundle_params.h"
#include "common/bal_projection.h"

class SnavelyReprojectionError {
public:
    SnavelyReprojectionError(double observation_x, double observation_y) :
            observed_x(observation_x), observed_y(observation_y) {}

    template<typename T>
    bool operator()(const T *const camera, const T *const point, T *residuals) const {
        // camera[0,1,2] are the angle-axis rotation
        T predictions[2];
        CamProjectionWithDistortion(camera, point, predictions);
        residuals[0] = predictions[0] - T(observed_x);
        residuals[1] = predictions[1] - T(observed_y);
        return true;
    }

    static ceres::CostFunction *Create(const double observed_x, const double observed_y) {
        return (new ceres::AutoDiffCostFunction<SnavelyReprojectionError, 2, 9, 3>(
                new SnavelyReprojectionError(observed_x, observed_y)));
    }

private:
    double observed_x;
    double observed_y;
};

void SetSolverOptionsFromFlags(BALProblem* bal_problem, const BundleParams& params, ceres::Solver::Options* options) {
    /// Set Minimizer Options
    options->max_num_iterations = params.num_iterations;
    options->minimizer_progress_to_stdout = true;
    options->num_threads = params.num_threads;
    // options->eta = params.eta;
    // options->max_solver_time_in_seconds = params.max_solver_time;
    CHECK(StringToTrustRegionStrategyType(params.trust_region_strategy, &options->trust_region_strategy_type));

    /// Set Linear Solver
    CHECK(ceres::StringToLinearSolverType(params.linear_solver, &options->linear_solver_type));
    CHECK(ceres::StringToSparseLinearAlgebraLibraryType(params.sparse_linear_algebra_library,
                                                        &options->sparse_linear_algebra_library_type));
    CHECK(ceres::StringToDenseLinearAlgebraLibraryType(params.dense_linear_algebra_library,
                                                       &options->dense_linear_algebra_library_type));
    options->num_linear_solver_threads = params.num_threads;

    /// Set Ordering
    if (params.ordering == "automatic")
        return;

    const int num_points        = bal_problem->num_points();
    const int num_cameras       = bal_problem->num_cameras();
    const int point_block_size  = bal_problem->point_block_size();
    const int camera_block_size = bal_problem->camera_block_size();
    double *points  = bal_problem->mutable_points();
    double *cameras = bal_problem->mutable_cameras();

    ceres::ParameterBlockOrdering *ordering = new ceres::ParameterBlockOrdering;
    // The points come before the cameras
    for (int i = 0; i < num_points; ++i)
        ordering->AddElementToGroup(points + point_block_size * i, 0);
    for (int i = 0; i < num_cameras; ++i)
        ordering->AddElementToGroup(cameras + camera_block_size * i, 1);
    options->linear_solver_ordering.reset(ordering);
}

void BuildProblem(BALProblem* bal_problem, ceres::Problem* problem, const BundleParams& params) {
    const int point_block_size  = bal_problem->point_block_size();
    const int camera_block_size = bal_problem->camera_block_size();

    double *points  = bal_problem->mutable_points();
    double *cameras = bal_problem->mutable_cameras();

    // Observations is 2 * num_observations long array observations [u_1, u_2, ... u_n],
    // where each u_i is two dimensional, the x and y position of the observation.
    const double *observations = bal_problem->observations();

    for (int i = 0; i < bal_problem->num_observations(); ++i) {
        ceres::CostFunction *cost_function =
                SnavelyReprojectionError::Create(observations[2 * i + 0], observations[2 * i + 1]);

        ceres::LossFunction *loss_function = params.robustify ? new ceres::HuberLoss(1.0) : NULL;

        double *camera = cameras + camera_block_size * bal_problem->camera_index()[i];
        double *point  = points  + point_block_size  * bal_problem->point_index()[i];

        // Each Residual block takes a point and a camera as input and outputs a 2 dimensional Residual
        problem->AddResidualBlock(cost_function, loss_function, camera, point);
    }
}

void SolveProblem(const char* filename, const BundleParams& params) {
    BALProblem bal_problem(filename);

    // show some information here ...
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
        ceres::Problem problem;
        BuildProblem(&bal_problem, &problem, params);
        ceres::Solver::Options options;
        SetSolverOptionsFromFlags(&bal_problem, params, &options);
        options.gradient_tolerance = 1e-16;
        options.function_tolerance = 1e-16;
        ceres::Solver::Summary summary;
        ceres::Solve(options, &problem, &summary);
        std::cout << "\n=== Ceres-Solver FullReport === \n" << summary.FullReport() << "\n";
    }

    // write the result into a .ply file.
    if (!params.final_ply.empty())
        bal_problem.WriteToPLYFile(params.final_ply);
}

int main(int argc, char** argv)
{
    BundleParams params(argc,argv);  // set the parameters here.

    google::InitGoogleLogging(argv[0]);

    std::cout << params.input << std::endl;
    if(params.input.empty()) {
        std::cout << "Usage: ba_ceres -input <path to bal_dataset: problem-*.txt>\n";
        return 1;
    }

    SolveProblem(params.input.c_str(), params);

    return 0;
}
