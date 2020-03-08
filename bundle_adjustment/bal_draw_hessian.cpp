#include <iostream>

#include "common/bal_problem.h"
#include "common/bundle_params.h"
#include "common/draw_hessian.h"

int main(int argc, char** argv)
{
    BundleParams params(argc,argv);  // set the parameters here.

    std::cout << params.input << std::endl;
    if(params.input.empty()) {
        std::cout << "Usage: bal_draw_hessian -input <path to bal_dataset: problem-*.txt>\n";
        return 1;
    }

    BALProblem bal_problem(params.input.c_str());

    std::cout << "\n\n"
              << "bal problem file loaded..." << std::endl
              << "bal problem have " << bal_problem.num_cameras() << " cameras and "
              << bal_problem.num_points() << " points. " << std::endl
              << "Forming " << bal_problem.num_observations() << " observatoins. " << std::endl
              << std::endl;

    DrawHessian(&bal_problem, 600);          

    return 0;
}
