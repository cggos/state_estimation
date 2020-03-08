#include <iostream>
#include <chrono>

#include <opencv2/core/core.hpp>

#include <ceres/ceres.h>

using namespace std;

struct CURVE_FITTING_COST {
    CURVE_FITTING_COST ( double x, double y ) : _x ( x ), _y ( y ) {}

    template <typename T>
    bool operator() ( const T* const abc, T* residual ) const {
        residual[0] = T(_y) - ceres::exp(abc[0] * T(_x) * T(_x) + abc[1] * T(_x) + abc[2]); // y-exp(ax^2+bx+c)
        return true;
    }
    const double _x, _y;
};

int main ( int argc, char** argv )
{
    int N=100;                          // 数据点
    double w_sigma=1.0;                 // 噪声Sigma值

    cv::RNG rng;                        // OpenCV随机数产生器
    double a=1.0, b=2.0, c=1.0;         // 真实参数值
    vector<double> x_data, y_data;

    for ( int i=0; i<N; i++ ) {
        double x = i / 100.0;
        x_data.push_back(x);
        y_data.push_back( exp(a * x * x + b * x + c) + rng.gaussian(w_sigma) );
    }

    double abc_e[3] = {0.0};

    ceres::Problem problem;
    for ( int i=0; i<N; i++ ) {
        problem.AddResidualBlock(
                new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 3>(new CURVE_FITTING_COST(x_data[i], y_data[i])),
                nullptr,            // 核函数，这里不使用，为空
                abc_e               // 待估计参数
        );
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;  // 增量方程如何求解
    options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

    ceres::Solve(options, &problem, &summary);

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout << "solve time cost = " << time_used.count() << " seconds. " <<endl;

    cout << summary.BriefReport() << endl;

    cout << "estimated a,b,c = ";
    for ( auto a : abc_e )
        cout << a << " ";
    cout << endl;

    return 0;
}

