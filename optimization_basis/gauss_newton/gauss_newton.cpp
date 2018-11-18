#include <iostream>
#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Cholesky>

using namespace std;
using namespace Eigen;

int main(int argc, char **argv) {

    double ar = 1.0, br = 2.0, cr = 1.0;         // 真实参数值
    double ae = 2.0, be = -1.0, ce = 5.0;        // 估计参数值

    int N = 100;                                 // 数据点
    double w_sigma = 1.0;                        // 噪声Sigma值
    cv::RNG rng;                                 // OpenCV随机数产生器

    vector<double> x_data, y_data;
    for (int i = 0; i < N; i++) {
        double x = i / 100.0;
        x_data.push_back(x);
        y_data.push_back(exp(ar * x * x + br * x + cr) + rng.gaussian(w_sigma));
    }

    // 开始Gauss-Newton迭代
    double xi, yi, ye, error, cost, lastCost;
    for (int iter = 0; iter < 100; iter++) {

        Vector3d J;
        Matrix3d H = Matrix3d::Zero();             // Hessian = J^T J in Gauss-Newton
        Vector3d b = Vector3d::Zero();             // bias

        cost = 0;

        for (int i = 0; i < N; i++) {
            xi = x_data[i];
            yi = y_data[i];

			ye = exp(ae*xi*xi + be*xi + ce);

            error = yi - ye;

            J[0] = -ye*xi*xi;  // de/da
            J[1] = -ye*xi;     // de/db
            J[2] = -ye;        // de/dc

            H += J * J.transpose(); // GN近似的H
            b += -error * J;

            cost += error * error;
        }

        // 求解线性方程 Hx=b，建议用ldlt
        Vector3d dx;
		dx = H.ldlt().solve(b);

        if (isnan(dx[0])) {
            cout << "result is nan!" << endl;
            break;
        }

        if (iter > 0 && cost > lastCost) {
            // 误差增长了，说明近似的不够好
            cout << "cost: " << cost << ", last cost: " << lastCost << endl;
            break;
        }

        // 更新abc估计值
        ae += dx[0];
        be += dx[1];
        ce += dx[2];

        lastCost = cost;

        cout << "total cost: " << cost << endl;
    }

    cout << "estimated abc = " << ae << ", " << be << ", " << ce << endl;
    return 0;
}
