#include <moveit/move_group_interface/move_group.h>
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

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

int main(int argc, char **argv)
{
  ros::init(argc, argv, "move_group_interface_demo", ros::init_options::AnonymousName);

  // start a ROS spinning thread
  ros::AsyncSpinner spinner(1);
  spinner.start();

  //visualization_msgs::Marker msg = ros::topic::waitForMessage<visualization_msgs::Marker("target");

  // this connecs to a running instance of the move_group node
  move_group_interface::MoveGroup group("manipulator");

  // specify that our target will be a random one
  //group.setRandomTarget();
  // plan the motion and then move the group to the sampled target 

  /* POSE 1 - just below target */
  group.setEndEffectorLink("link_7");

  //print ("Marker: %.2f", msg.pose.position.x);
  geometry_msgs::PoseStamped target;
  //geometry_msgs::PoseStamped orientation;
  target=group.getCurrentPose("link_7");

  //target=group.getCurrentPose("link_7");

  ROS_INFO ("Position: %f %f %f", target.pose.position.x, target.pose.position.y, target.pose.position.z);
  ROS_INFO ("Oreitation: %f %f %f %f", target.pose.orientation.x, target.pose.orientation.y, target.pose.orientation.z, target.pose.orientation.w);


  group.setPositionTarget(target.pose.position.x,target.pose.position.y,target.pose.position.z-0.1,"link_7");

  group.move();

  ros::Duration(3).sleep(); // sleep for 3 a second

  ros::waitForShutdown();
}
