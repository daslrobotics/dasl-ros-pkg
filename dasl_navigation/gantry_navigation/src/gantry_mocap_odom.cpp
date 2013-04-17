#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "gantry_odom");

  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  tf::TransformListener listener;

  listener.waitForTransform("/mocap_origin", "/gantry_base", ros::Time::now(), ros::Duration(3.0));

  ros::Rate r(30.0);

  while(n.ok()){

    //ros::spinOnce();               		// check for incoming messages
    current_time = ros::Time::now();

    tf::StampedTransform transform;
    geometry_msgs::Twist vel_msg;
    
    listener.lookupTransform("/mocap_origin", "/gantry_base",
                             ros::Time(0), transform);

    listener.lookupTwist("/mocap_origin", "/gantry_base", 
                         ros::Time(0), ros::Duration(0.1), vel_msg);

    //publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "gantry_base";

    odom_trans.transform.translation.x = transform.getOrigin().x();
    odom_trans.transform.translation.y = transform.getOrigin().y();
    odom_trans.transform.translation.z = transform.getOrigin().z();
    odom_trans.transform.rotation.x = transform.getRotation().x();
    odom_trans.transform.rotation.y = transform.getRotation().y();
    odom_trans.transform.rotation.z = transform.getRotation().z();
    odom_trans.transform.rotation.w = transform.getRotation().w();

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = transform.getOrigin().x();
    odom.pose.pose.position.y = transform.getOrigin().y();
    odom.pose.pose.position.z = transform.getOrigin().z();
    odom.pose.pose.orientation.x = transform.getRotation().x();
    odom.pose.pose.orientation.y = transform.getRotation().y();
    odom.pose.pose.orientation.z = transform.getRotation().z();
    odom.pose.pose.orientation.w = transform.getRotation().w();

    //set the velocity
    odom.child_frame_id = "gantry_base";
    odom.twist.twist.linear.x = -vel_msg.linear.x;
    odom.twist.twist.linear.y = -vel_msg.linear.y;
    odom.twist.twist.linear.z = -vel_msg.linear.z;
    odom.twist.twist.angular.x = vel_msg.angular.x;
    odom.twist.twist.angular.y = vel_msg.angular.y;
    odom.twist.twist.angular.z = vel_msg.angular.z;

    //publish the message
    odom_pub.publish(odom);

    //last_time = current_time;
    r.sleep();
  }
}

