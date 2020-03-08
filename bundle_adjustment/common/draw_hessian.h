#ifndef SLAM_OPTIMIZATION_DRAW_HESSIAN_H
#define SLAM_OPTIMIZATION_DRAW_HESSIAN_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <Eigen/Core>

void DrawHessian(const BALProblem* bal_problem, const int max_point_id = 1000) {

    const int num_points        = bal_problem->num_points();
    const int num_cameras       = bal_problem->num_cameras();
    const int num_observations  = bal_problem->num_observations();

    std::vector<int> idx_obs;
    idx_obs.reserve(num_observations);
    for (int i = 0; i < num_observations; ++i) {
        const int camera_id = bal_problem->camera_index()[i];
        const int point_id = bal_problem->point_index()[i];

        if (point_id >= max_point_id)
            continue;

        idx_obs.push_back(i);
    }

    const int j_rows = idx_obs.size();
    const int j_cols = num_cameras + max_point_id;

    Eigen::MatrixXi mJ(j_rows, j_cols);
    mJ.setZero();

    for (int i = 0; i < idx_obs.size(); ++i) {
        int j = idx_obs[i];
        const int camera_id = bal_problem->camera_index()[j];
        const int point_id = bal_problem->point_index()[j];
        // std::cout << camera_id << ", " << point_id << std::endl;
        mJ(i, camera_id) = 1;
        mJ(i, num_cameras + point_id) = 1;
    }

    std::cout << "DrawHessian: mJ size is (" << mJ.rows() << ", " << mJ.cols() << ")" << std::endl;

    Eigen::MatrixXi mH = mJ.transpose() * mJ;

    cv::Mat mat_H(mH.rows(), mH.cols(), CV_8UC1);
    for (int y = 0; y < mH.rows(); ++y) {
        for (int x = 0; x < mH.cols(); ++x) {
            mat_H.at<unsigned char>(y, x) = mH(y, x);
        }
    }
    cv::threshold(mat_H, mat_H, 0, 255, cv::THRESH_BINARY);
    cv::imshow("mat_H", mat_H);
    cv::waitKey(0);
}

#endif //SLAM_OPTIMIZATION_DRAW_HESSIAN_H
