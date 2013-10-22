#include <moveit/move_group_interface/move_group.h>
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Quaternion.h>
#include <nav_msgs/Odometry.h>
// MoveIt!
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/GetStateValidity.h>
#include <moveit_msgs/DisplayRobotState.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>

// Visualization marker
 #include <eigen_conversions/eigen_msg.h>
#include <visualization_msgs/Marker.h>
#include <tf/tfMessage.h>
double xt;
double yt;
double zt;
double xb;
double yb;
double zb;
double xe;
double ze;
double ye;
double y_new;
double z_new;
float c=0.0;
int a;
int t=0,t1=0;
void track_hand(const  geometry_msgs::Quaternion::ConstPtr& pos)
{

xt=pos->x;
yt=pos->y;
zt=pos->z;
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "follow_marker", ros::init_options::AnonymousName);

  // start a ROS spinning thread
  ros::AsyncSpinner spinner(1);
ros::NodeHandle nh;
//ros::Subscriber click=nh.subscribe("coor",10,track_hand);
  spinner.start();
nav_msgs::Odometry enter_coor;

// this connecs to a running instance of the move_group node
move_group_interface::MoveGroup group("manipulator");

group.setEndEffectorLink("link_7");

//group.setOrientationTarget(-0.004,-0.02,0.02,0.99,"link_7");

//group.setOrientationTarget(0,0,0,1,"link_7");
//group.setOrientationTarget(-0.004,-0.01,0.02,0.99,"link_7");
group.setPlannerId("RRTConnectkConfigDefault");
//group.setOrientationTarget(0.0,0.0,0.0,1.0,"link_7");
enter_coor.pose.pose.position.x=0.3;
 enter_coor.pose.pose.position.y= -0.6287;
 enter_coor.pose.pose.position.z=  -0.88;
 enter_coor.pose.pose.orientation.x=0.0;
 enter_coor.pose.pose.orientation.y=0.0;
 enter_coor.pose.pose.orientation.z=0.0;
enter_coor.pose.pose.orientation.w=1.0;
 Eigen::Affine3d end_effector_state;

tf::poseMsgToEigen(enter_coor.pose.pose,end_effector_state);
group.setPoseTarget(end_effector_state,"link_7");
group.move();
ROS_INFO("Moved to position to pos 1");
enter_coor.pose.pose.position.x=0.3;
 enter_coor.pose.pose.position.y= -0.6287;
 enter_coor.pose.pose.position.z=  -0.48;
 enter_coor.pose.pose.orientation.x=0.0;
 enter_coor.pose.pose.orientation.y=0.0;
 enter_coor.pose.pose.orientation.z=0.0;
enter_coor.pose.pose.orientation.w=1.0;
 

tf::poseMsgToEigen(enter_coor.pose.pose,end_effector_state);
group.setPoseTarget(end_effector_state,"link_7");
group.move();
ROS_INFO("Moved to position to pos 2");
enter_coor.pose.pose.position.x=0.3;
 enter_coor.pose.pose.position.y= -0.11;
 enter_coor.pose.pose.position.z=  -0.48;
 enter_coor.pose.pose.orientation.x=0.0;
 enter_coor.pose.pose.orientation.y=0.0;
 enter_coor.pose.pose.orientation.z=0.0;
enter_coor.pose.pose.orientation.w=1.0;
 

tf::poseMsgToEigen(enter_coor.pose.pose,end_effector_state);
group.setPoseTarget(end_effector_state,"link_7");
group.move();
ROS_INFO("Moved to position to pos 3");

enter_coor.pose.pose.position.x=0.3;
 enter_coor.pose.pose.position.y= -0.11;
 enter_coor.pose.pose.position.z=  -0.915;
 enter_coor.pose.pose.orientation.x=0.0;
 enter_coor.pose.pose.orientation.y=0.0;
 enter_coor.pose.pose.orientation.z=0.0;
enter_coor.pose.pose.orientation.w=1.0;
 

tf::poseMsgToEigen(enter_coor.pose.pose,end_effector_state);
group.setPoseTarget(end_effector_state,"link_7");
group.move();
ROS_INFO("Moved to position to pos 4");
/*
group.setPositionTarget(0.3,-0.6287,-0.88,"link_7"); 
group.move();
ROS_INFO("Moved to position to pos 1");
group.setPositionTarget(0.3,-0.6287,-0.48,"link_7"); 
group.move();
ROS_INFO("Moved to position to pos 2");

group.setPositionTarget(0.3,-0.11,-0.48,"link_7"); 
group.move();
ROS_INFO("Moved to position to pos 3");

group.setPositionTarget(0.3,-0.1104,-0.915,"link_7"); 
group.move();
ROS_INFO("Moved to position to pos 4");

group.setPositionTarget(0.3,-0.6287,-0.88,"link_7"); 
group.move();
ROS_INFO("Moved to position to pos 5");
*/
while(nh.ok())
{
//group.setPositionTarget(0.3,-0.2495,-0.6854,"link_7"); // This is for making the robot follow the mocap marker.

////group.setOrientationTarget(-0.00874376491464,-0.0184065077432,0.0196593480627,0.99959904816,"link_7");
//group.setPositionTarget(0.24,0.119,1.076,"link_7"); // This is for making the robot follow the mocap marker.
//group.setOrientationTarget(-0.004,-0.01,0.02,0.99,"link_7");
//group.setOrientationTarget(-0.004,-0.02,0.02,0.99,"link_7");
//std::cout<<"here:1"<<"\n";
// group.setPositionTarget(a,b,c,"link_name")
//a-forward move
//b-lateral movement
//c-up and down(altitude)
//group.setPositionTarget(0.24,0.409,1.071,"link_7");
//group.move();
ros::Duration(0.7).sleep(); 
}

  ros::waitForShutdown();
}



