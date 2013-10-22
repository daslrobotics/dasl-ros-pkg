#include <moveit/move_group_interface/move_group.h>
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Quaternion.h>
// MoveIt!
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/GetStateValidity.h>
#include <moveit_msgs/DisplayRobotState.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>

// Visualization marker
#include <visualization_msgs/Marker.h>
#include <tf/tfMessage.h>
double xt;
double yt;
double zt;
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
ros::Subscriber click=nh.subscribe("coor",10,track_hand);
  spinner.start();
geometry_msgs::PoseStamped target;
 

  // this connecs to a running instance of the move_group node
 move_group_interface::MoveGroup group("manipulator");

 
  group.setEndEffectorLink("link_7");

while(nh.ok())
{
  group.setPositionTarget (xt,yt,1.28, "link_7");

 group.move();

  ros::Duration(0.4).sleep(); // sleep for 3 a second
}





  ros::waitForShutdown();
}



