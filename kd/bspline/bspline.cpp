#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <sophus/se3.hpp>
#include <sophus/so3.hpp>

void get_pose(double timestamp, Eigen::Matrix<double, 4, 4> &Twi) {
  // absolute four control poses
  double angle_theta = M_PI / 2;

  Eigen::Matrix3d R = Eigen::AngleAxisd(-angle_theta, Eigen::Vector3d(0, 0, 1)).toRotationMatrix();
  Sophus::SE3d T_w_im1(R, Eigen::Vector3d(cos(-angle_theta), sin(-angle_theta), 0));

  R = Eigen::AngleAxisd(0, Eigen::Vector3d(0, 0, 1)).toRotationMatrix();
  Sophus::SE3d T_w_i(R, Eigen::Vector3d(1, 0, 0));

  R = Eigen::AngleAxisd(+angle_theta, Eigen::Vector3d(0, 0, 1)).toRotationMatrix();
  Sophus::SE3d T_w_ip1(R, Eigen::Vector3d(cos(+angle_theta), sin(+angle_theta), 0));

  R = Eigen::AngleAxisd(2 * angle_theta, Eigen::Vector3d(0, 0, 1)).toRotationMatrix();
  Sophus::SE3d T_w_ip2(R, Eigen::Vector3d(cos(2 * angle_theta), sin(2 * angle_theta), 0));

  // incremental control poses
  Sophus::Vector6d Omege_im1_i = (T_w_im1.inverse() * T_w_i).log();
  Sophus::Vector6d Omege_i_ip1 = (T_w_i.inverse() * T_w_ip1).log();
  Sophus::Vector6d Omege_ip1_ip2 = (T_w_ip1.inverse() * T_w_ip2).log();

  double delta_t = 1 * angle_theta / M_PI;
  double u = timestamp / delta_t;

  double B_0 = 1.0 / 6.0 * (5 + 3 * u - 3 * u * u + 1 * u * u * u);
  double B_1 = 1.0 / 6.0 * (1 + 3 * u + 3 * u * u - 2 * u * u * u);
  double B_2 = 1.0 / 6.0 * (u * u * u);

  double dB_0 = 1.0 / (6 * delta_t) * (3 - 6 * u + 3 * u * u);
  double dB_1 = 1.0 / (6 * delta_t) * (3 + 6 * u - 6 * u * u);
  double dB_2 = 1.0 / (6 * delta_t) * (3 * u * u);

  double ddB_0 = 1.0 / (6 * delta_t * delta_t) * (-6 + 6 * u);
  double ddB_1 = 1.0 / (6 * delta_t * delta_t) * (6 - 12 * u);
  double ddB_2 = 1.0 / (6 * delta_t * delta_t) * (6 * u);

  Eigen::Matrix<double, 4, 4> A_0 = Sophus::SE3d::exp(B_0 * Omege_im1_i).matrix();
  Eigen::Matrix<double, 4, 4> A_1 = Sophus::SE3d::exp(B_1 * Omege_i_ip1).matrix();
  Eigen::Matrix<double, 4, 4> A_2 = Sophus::SE3d::exp(B_2 * Omege_ip1_ip2).matrix();

  Eigen::Matrix<double, 4, 4> dA_0 = dB_0 * Sophus::SE3d::hat(Omege_im1_i).matrix() * A_0;
  Eigen::Matrix<double, 4, 4> dA_1 = dB_1 * Sophus::SE3d::hat(Omege_i_ip1).matrix() * A_1;
  Eigen::Matrix<double, 4, 4> dA_2 = dB_2 * Sophus::SE3d::hat(Omege_ip1_ip2).matrix() * A_2;
  // Eigen::Matrix<double, 4, 4> dA_1 = dB_1 * A_1 * Sophus::SE3::hat(Omege_i_ip1).matrix();
  // Eigen::Matrix<double, 4, 4> dA_2 = dB_2 * A_2 * Sophus::SE3::hat(Omege_ip1_ip2).matrix();

  Eigen::Matrix<double, 4, 4> ddA_0 =
      dB_0 * Sophus::SE3d::hat(Omege_im1_i).matrix() * dA_0 + ddB_0 * Sophus::SE3d::hat(Omege_im1_i).matrix() * A_0;
  Eigen::Matrix<double, 4, 4> ddA_1 =
      dB_1 * Sophus::SE3d::hat(Omege_i_ip1).matrix() * dA_1 + ddB_1 * Sophus::SE3d::hat(Omege_i_ip1).matrix() * A_1;
  Eigen::Matrix<double, 4, 4> ddA_2 =
      dB_2 * Sophus::SE3d::hat(Omege_ip1_ip2).matrix() * dA_2 + ddB_2 * Sophus::SE3d::hat(Omege_ip1_ip2).matrix() * A_2;
  // Eigen::Matrix<double, 4, 4> ddA_1 = dB_1 * dA_1 * Sophus::SE3::hat(Omege_i_ip1).matrix() + ddB_1 * A_1 *
  // Sophus::SE3::hat(Omege_i_ip1).matrix(); Eigen::Matrix<double, 4, 4> ddA_2 = dB_2 * dA_2 *
  // Sophus::SE3::hat(Omege_ip1_ip2).matrix() + ddB_2 * A_2 * Sophus::SE3::hat(Omege_ip1_ip2).matrix();

  Eigen::Matrix<double, 4, 4> T_w_s = T_w_im1.matrix() * A_0 * A_1 * A_2;

  Eigen::Matrix<double, 4, 4> dT_w_s = T_w_im1.matrix() * (dA_0 * A_1 * A_2 + A_0 * dA_1 * A_2 + A_0 * A_1 * dA_2);
  Eigen::Matrix<double, 4, 4> ddT_w_s =
      T_w_im1.matrix() * (ddA_0 * A_1 * A_2 + A_0 * ddA_1 * A_2 + A_0 * A_1 * ddA_2 +
                          2 * (dA_0 * dA_1 * A_2 + A_0 * dA_1 * dA_2 + dA_0 * A_1 * dA_2));

  Eigen::Matrix<double, 3, 3> R_g_i = T_w_s.block(0, 0, 3, 3);
  Eigen::Matrix<double, 3, 3> dR_g_i = dT_w_s.block(0, 0, 3, 3);
  Eigen::Vector3d ddp_g_i = ddT_w_s.block<3, 1>(0, 3);

  Eigen::Vector3d G(0, 0, 9.81);

  // Output:
  Eigen::Vector3d imu_gyro = Sophus::SO3d::vee(R_g_i.transpose() * dR_g_i);
  Eigen::Vector3d imu_acc = R_g_i.transpose() * (ddp_g_i - G);

  Twi = T_w_s;
}

int main() {
  Eigen::Matrix<double, 4, 4> Twi;
  get_pose(0.1, Twi);

  std::cout << Twi << std::endl;

  return 0;
}