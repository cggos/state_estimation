#include "ros_tf_utils.h"

int main()
{
    Eigen::AngleAxisd eg_ax ( M_PI/4, Eigen::Vector3d ( 0,0,1 ) );
    Eigen::Matrix3d eg_m3_r = eg_ax.toRotationMatrix();

    Eigen::Matrix4d eg_m4 = Eigen::Matrix4d::Identity();
    eg_m4.topLeftCorner(3,3) = eg_m3_r;

    tf::Transform tf;
    cg::RosTf::eigen2tf(eg_m4, tf);

    Eigen::Matrix4d eg_m4_2 = Eigen::Matrix4d::Identity();
    cg::RosTf::tf2eigen(tf, eg_m4_2);

    std::cout << "eg_m4: \n" << eg_m4 << std::endl;

    cg::RosTf::print_tf(tf, "tf");

    std::cout << "eg_m4_2: \n" << eg_m4_2 << std::endl;

    return 0;
}

