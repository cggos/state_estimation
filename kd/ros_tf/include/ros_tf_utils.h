#ifndef ROS_TF_UTILS_H
#define ROS_TF_UTILS_H

#include <iostream>
#include <iomanip>

#include <ros/ros.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>

#include <nav_msgs/Odometry.h>

#include <tf/transform_listener.h>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_ros/transform_listener.h>

#include <tf_conversions/tf_eigen.h>

#include <Eigen/Geometry>

namespace cg {

    class RosTf {

    public:

        void lookup_tf(std::string str_from, std::string str_to, tf::StampedTransform &tf) {

            std::string msg_error = "";
            ros::Time timestamp = ros::Time(0);

            if(tf_listener_.canTransform(str_to, str_from, timestamp, &msg_error)) {
                tf_listener_.lookupTransform(str_to, str_from, timestamp, tf);
            } else {
                ROS_WARN_THROTTLE(10.0,
                                  "The tf from '%s' to '%s' does not seem to be available, will assume it as identity!",
                                  str_from.c_str(), str_to.c_str());
                ROS_ERROR("tf error: %s", msg_error.c_str());
                tf.setIdentity();
            }
        }

        void lookup_tf2(std::string str_from, std::string str_to, geometry_msgs::TransformStamped &tf2) {

            try {
                if(tf2_buffer_->canTransform(str_to, str_from, ros::Time(0)))
                    tf2 = tf2_buffer_->lookupTransform(str_to, str_from, ros::Time(0));
                else
                    ROS_ERROR("tf2 error: cannot transform from %s to %s", str_from.c_str(), str_to.c_str());
            }
            catch (tf2::TransformException &ex) {
                ROS_ERROR("tf2 TransformException: %s", ex.what());
                ros::Duration(1.0).sleep();
            }
        }

        static void print_tf(const tf::Transform &tf, std::string str_msg) {

            double rr, rp, ry;
            tf.getBasis().getRPY(rr, rp, ry);
            rr = rr * M_1_PI * 180.0;
            rp = rp * M_1_PI * 180.0;
            ry = ry * M_1_PI * 180.0;

            tf::Quaternion tf_q4 = tf.getRotation();
            tf::Vector3    tf_v3 = tf.getOrigin();

            double px = tf_v3.getX();
            double py = tf_v3.getY();
            double pz = tf_v3.getZ();

            double qx = tf_q4.getX();
            double qy = tf_q4.getY();
            double qz = tf_q4.getZ();
            double qw = tf_q4.getW();

            printf(
                    "\n=================================================\n"\
                    "%s tf :\n"\
                        "  Translation: [%f, %f, %f]\n"\
                        "  Rotation:\n"\
                        "    in Quaternion:  [%f, %f, %f, %f]\n"\
                        "    in RPY(degree): [%f, %f, %f]\n\n",
                    str_msg.c_str(), px, py, pz, qx, qy, qz, qw, rr, rp, ry);
        }

        static void gmpose2tf(const geometry_msgs::Pose &gm_pose, tf::Transform &tf) {

            geometry_msgs::Point      gm_pt = gm_pose.position;
            geometry_msgs::Quaternion gm_q4 = gm_pose.orientation;

            tf::Vector3 tf_v3;
            tf_v3.setX(gm_pt.x);
            tf_v3.setY(gm_pt.y);
            tf_v3.setZ(gm_pt.z);

            tf::Quaternion tf_q4;
            tf_q4.setX(gm_q4.x);
            tf_q4.setY(gm_q4.y);
            tf_q4.setZ(gm_q4.z);
            tf_q4.setW(gm_q4.w);

            tf.setIdentity();
            tf.setOrigin(tf_v3);
            tf.setRotation(tf_q4);
        }

        static void gmtf2eigen(const geometry_msgs::Transform &gm_tf, Eigen::Matrix4d &eg_m4) {

            geometry_msgs::Quaternion gm_q4 = gm_tf.rotation;
            geometry_msgs::Vector3    gm_v3 = gm_tf.translation;

            Eigen::Quaterniond eg_q4(gm_q4.w, gm_q4.x, gm_q4.y, gm_q4.z);
            Eigen::Vector3d    eg_v3(gm_v3.x, gm_v3.y, gm_v3.z);

            eg_m4 = Eigen::Matrix4d::Identity();
            eg_m4.block<3,3>(0,0) = eg_q4.matrix();
            eg_m4.block<3,1>(0,3) = eg_v3;
        }

        static void eigen2tf(const Eigen::Matrix4d &eg_m4, tf::Transform &tf) {
            Eigen::Affine3d affine(eg_m4);
            tf::transformEigenToTF(affine, tf);
        }

        static void tf2eigen(const tf::Transform &tf, Eigen::Matrix4d &eg_m4) {

            tf::Quaternion tf_q4 = tf.getRotation();
            tf::Vector3    tf_v3 = tf.getOrigin();

            Eigen::Quaterniond eg_q4;
            Eigen::Vector3d    eg_v3;

            tf::vectorTFToEigen(tf_v3, eg_v3);
            tf::quaternionTFToEigen(tf_q4, eg_q4);

            eg_m4 = Eigen::Matrix4d::Identity();
            eg_m4.block<3,3>(0,0) = eg_q4.matrix();
            eg_m4.block<3,1>(0,3) = eg_v3;
        }

    private:
        tf::TransformListener tf_listener_;

        boost::shared_ptr<tf2_ros::Buffer> tf2_buffer_;
        boost::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
    };

}

#endif // ROS_TF_UTILS_H
