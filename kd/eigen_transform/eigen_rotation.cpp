#include <ceres/rotation.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <sophus/so3.hpp>

#define DEG2RAD (M_PI / 180)
#define RAD2DEG (180 / M_PI)

Eigen::Quaterniond YXZ(const Eigen::Vector3d &attitude) {
  Eigen::AngleAxisd Y(attitude.y(), Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd X(attitude.x(), Eigen::Vector3d::UnitX());
  Eigen::AngleAxisd Z(attitude.z(), Eigen::Vector3d::UnitZ());
  return Y * X * Z;
}

/**
 * @brief Eigen::UnitRandom(), ref: http://planning.cs.uiuc.edu/node198.html
 * @return
 */
Eigen::Quaterniond unit_random() {
    double u1 = rand() / double(RAND_MAX);  // [0, 1]
    double u2 = rand() / double(RAND_MAX) * M_2_PI;
    double u3 = rand() / double(RAND_MAX) * M_2_PI;
    double a = std::sqrt(1 - u1);
    double b = std::sqrt(u1);
    return Eigen::Quaterniond(a * sin(u2), a * cos(u2), b * sin(u3), b * cos(u3)).normalized();
}

int main() {

    double pitch = 2.3 * DEG2RAD;
    double roll = 0 * DEG2RAD;
    double yaw = 0 * DEG2RAD;

    double pitch1 = 2.5 * DEG2RAD;
    double roll1 = 0 * DEG2RAD;
    double yaw1 = 0 * DEG2RAD;

    Eigen::Vector3d start = Eigen::Vector3d(pitch, roll, yaw);
    Eigen::Vector3d end = Eigen::Vector3d(pitch1, roll1, yaw1);

    Eigen::Quaterniond delta = YXZ(end) * YXZ(start).inverse();

    printf("x = %f\n", delta.matrix().eulerAngles(1, 0, 2).x() * RAD2DEG);
    printf("y = %f\n", delta.matrix().eulerAngles(1, 0, 2).y() * RAD2DEG);
    printf("z = %f\n", delta.matrix().eulerAngles(1, 0, 2).z() * RAD2DEG);
    std::cout << std::endl;

    Eigen::Matrix3f mm;

    mm = Eigen::AngleAxisf(0.25*M_PI, Eigen::Vector3f::UnitX())
      * Eigen::AngleAxisf(0.55*M_PI, Eigen::Vector3f::UnitY())
      * Eigen::AngleAxisf(0.33*M_PI, Eigen::Vector3f::UnitZ());

    std::cout << "original rotation:" << std::endl;
    std::cout << mm << std::endl << std::endl;

    Eigen::Vector3f ea = mm.eulerAngles(0, 1, 2); 
    std::cout << "to Euler angles:" << std::endl;
    std::cout << ea << std::endl << std::endl;

    Eigen::Matrix3f nn;
    nn = Eigen::AngleAxisf(ea[0], Eigen::Vector3f::UnitX())
      * Eigen::AngleAxisf(ea[1], Eigen::Vector3f::UnitY())
      * Eigen::AngleAxisf(ea[2], Eigen::Vector3f::UnitZ()); 

    std::cout << "recalc original rotation:" << std::endl;
    std::cout << nn << std::endl;
    std::cout << std::endl;

    //------------------------------------------------------------------------------------

    // Eigen::Matrix3d R = Eigen::AngleAxisd(M_PI_2, Eigen::Vector3d(0,0,1)).toRotationMatrix();
    Eigen::Matrix3d R(unit_random());
    std::cout << "the random initial rotation matrix R:\n"
              << R << std::endl
              << std::endl;

    Sophus::SO3d R_SO3(R);
    std::cout << "R_SO3:\n"
              << R_SO3.matrix() << std::endl
              << std::endl;

    Eigen::Quaterniond R_q(R);

    //------------------------------------------------------------------------------------

    Eigen::Quaterniond q0(R);
    Eigen::Quaterniond q1(R.transpose());
    Eigen::Quaterniond q2 = q0.inverse();
    std::cout << "q0: " << q0.coeffs().transpose() << std::endl;
    std::cout << "q1: " << q1.coeffs().transpose() << std::endl;
    std::cout << "q2: " << q2.coeffs().transpose() << std::endl;
    Eigen::Matrix3d Rq1 = q1.toRotationMatrix();
    Eigen::Matrix3d Rq2 = q2.toRotationMatrix();
    std::cout << "Rq1:\n"
              << Rq1 << std::endl;
    std::cout << "Rq2:\n"
              << Rq2 << std::endl;
    std::cout << std::endl;

    //------------------------------------------------------------------------------------

    Eigen::Vector3d omega(0.01, 0.02, 0.03);
    std::cout << "omega: "
              << omega.transpose() << std::endl
              << std::endl;

    //------------------------------------------------------------------------------------

    Sophus::SO3d SO3_omega = Sophus::SO3d::exp(omega);
    Eigen::AngleAxisd angle_axis(SO3_omega.matrix());
    std::cout << "angle_axis: "
              << angle_axis.angle() * angle_axis.axis().transpose() << std::endl
              << std::endl;

    //------------------------------------------------------------------------------------

    omega *= 10;

    Eigen::Vector3d v30 = R_SO3.log();

    Eigen::Vector3d vec_plus = v30 + omega;

    Sophus::SO3d R_SO3_plus = Sophus::SO3d::exp(vec_plus);
    Eigen::Matrix3d R00 = R_SO3_plus.matrix();

    Eigen::Quaterniond q_plus(Eigen::AngleAxisd(vec_plus.norm(), vec_plus.normalized()));
    Eigen::Matrix3d R01 = q_plus.matrix();

    Sophus::SO3d R_SO3_updated = R_SO3 * Sophus::SO3d::exp(omega);
    Eigen::Matrix3d R10 = R_SO3_updated.matrix();

    Eigen::Quaterniond q_update(Eigen::AngleAxisd(omega.norm(), omega.normalized()));
    // q_update.w() = 1;
    // q_update.vec() = 0.5 * omega;
    Eigen::Quaterniond R_q_updated = (R_q * q_update).normalized();
    Eigen::Matrix3d R11 = R_q_updated.toRotationMatrix();

    std::cout << "R00:\n"
              << R00 << std::endl
              << std::endl;
    std::cout << "R01:\n"
              << R01 << std::endl
              << std::endl;
    std::cout << "R10:\n"
              << R10 << std::endl
              << std::endl;
    std::cout << "R11:\n"
              << R11 << std::endl
              << std::endl;

    std::cout << "R00-R01 norm: " << (R00-R01).norm() << std::endl;
    std::cout << "R00-R10 norm: " << (R00-R10).norm() << std::endl;
    std::cout << "R10-R11 norm: " << (R10-R11).norm() << std::endl;

    // std::cout << "the Frobenius Norm of the error matrix: " << R_error.norm() << std::endl;
    // std::cout << "the sum of the error matrix: " << R_error.sum() << std::endl;

    std::cout << std::endl;

    //------------------------------------------------------------------------------------

    Eigen::Vector3d P(1.0, 2.0, 3.0);
    Eigen::Vector3d P1 = R * P;

    Eigen::Quaterniond Pq;
    Pq.w() = 0.0;
    Pq.vec() = P;
    Eigen::Quaterniond P2q = R_q * Pq * R_q.inverse();
    Eigen::Vector3d P2 = P2q.vec();

    Eigen::Vector3d P3 = R_q * P;

    std::cout << "P1: " << P1.transpose() << std::endl;
    std::cout << "P2: " << P2.transpose() << std::endl;
    std::cout << "P3: " << P3.transpose() << std::endl;

    std::cout << std::endl;

    //------------------------------------------------------------------------------------

    double q_ceres[4];
    ceres::RotationMatrixToQuaternion<double>(R.data(), q_ceres);
    std::cout << "R --> q (Ceres): " << q_ceres[1] << ", " << q_ceres[2] << ", " << q_ceres[3] << ", " << q_ceres[0] << std::endl;
    Eigen::Quaterniond q_eigen(R);
    std::cout << "R --> q (Eigen): " << q_eigen.coeffs().transpose() << std::endl;

    return 0;
}
