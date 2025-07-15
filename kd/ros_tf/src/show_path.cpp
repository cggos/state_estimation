#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <nav_msgs/Path.h>
#include <ros/console.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "showpath");

    ros::NodeHandle ph;
    ros::Publisher path_pub = ph.advertise<nav_msgs::Path>("trajectory", 1, true);

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();

    nav_msgs::Path path;
    //nav_msgs::Path path;
    path.header.stamp = current_time;
    path.header.frame_id = "odom";

    double x = 0.0;
    double y = 0.0;
    double th = 0.0;
    double vx = 1;
    double vy = 1;
    double vth = 0.1;

    double dt = 1.;

    double delta_th = 0.0;
    double delta_x = 0.0;
    double delta_y = 0.0;

    ros::Rate loop_rate(1);
    while (ros::ok()) {
        current_time = ros::Time::now();
        //compute odometry in a typical way given the velocities of the robot        

        geometry_msgs::PoseStamped this_pose_stamped;
        this_pose_stamped.pose.position.x = x;
        this_pose_stamped.pose.position.y = y;

        geometry_msgs::Quaternion goal_quat = tf::createQuaternionMsgFromYaw(th);
        this_pose_stamped.pose.orientation.x = goal_quat.x;
        this_pose_stamped.pose.orientation.y = goal_quat.y;
        this_pose_stamped.pose.orientation.z = goal_quat.z;
        this_pose_stamped.pose.orientation.w = goal_quat.w;

        // double r = 5;
        // x = r * cos(th);
        // y = r * sin(th);  

        // why
        x += (vx * cos(th) - vy * sin(th)) * dt;
        y += (vx * sin(th) + vy * cos(th)) * dt;

        th += vth * dt;

        this_pose_stamped.header.stamp = current_time;
        this_pose_stamped.header.frame_id = "odom";
        path.poses.push_back(this_pose_stamped);

        path_pub.publish(path);
        ros::spinOnce();  // check for incoming messages

        last_time = current_time;
        loop_rate.sleep();
    }

    return 0;
}
