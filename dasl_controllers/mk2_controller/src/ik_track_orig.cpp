/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2012, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of Willow Garage, Inc. nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: Sachin Chitta
*********************************************************************/

#include <ros/ros.h>

// MoveIt!
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/joint_state_group.h>

// Track Marker
#include <visualization_msgs/Marker.h>

// Nav Messages
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64.h>
#include <eigen_conversions/eigen_msg.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/Joy.h>

// Defines
nav_msgs::Odometry tracked_object_;
nav_msgs::Odometry target;

sensor_msgs::Joy joy_data_;
geometry_msgs::Pose actual_end_effector_;
geometry_msgs::Pose desired_end_effector_;

Eigen::Affine3d end_effector_target_;
bool object_msg_received_;
void new_odom_cb_(const nav_msgs::OdometryConstPtr& msg);
void joy_cb_(const sensor_msgs::Joy::ConstPtr& joy);

void new_odom_cb_(const nav_msgs::OdometryConstPtr& msg)
{
  //update the target
  if (msg->child_frame_id == "target")
  {
    tracked_object_.pose.pose.position.x = 0.35; //msg->pose.pose.position.x - 0.4;
    tracked_object_.pose.pose.position.y = msg->pose.pose.position.y - 1.0;
    tracked_object_.pose.pose.position.z = msg->pose.pose.position.z - 1.92;

    tracked_object_.twist = msg->twist;
    tracked_object_.child_frame_id = msg->child_frame_id;
    tracked_object_.header = msg->header;

    //Fixed orientation of the wrist
    tracked_object_.pose.pose.orientation.w = 1.0;
    tracked_object_.pose.pose.orientation.x = 0.0;
    tracked_object_.pose.pose.orientation.y = 0.0;
    tracked_object_.pose.pose.orientation.z = 0.0;

    object_msg_received_ = true;
  }
}

void joy_cb_(const sensor_msgs::Joy::ConstPtr& joy)
{
  joy_data_.axes[0] = joy->axes[0];
  joy_data_.axes[1] = joy->axes[1];
  joy_data_.axes[2] = joy->axes[2];
  joy_data_.buttons[0] = joy->buttons[0];
  //ROS_INFO_STREAM("Joy: " << joy->axes[1]);
}

int main(int argc, char **argv)
{
  ros::init (argc, argv, "arm_kinematics");
  ros::AsyncSpinner spinner(1);
  spinner.start();

  ros::Subscriber odom_sub_;
  ros::Subscriber joy_sub_;
  ros::Publisher joint_pub_;
  ros::NodeHandle nh_tilde_;

  odom_sub_ = nh_tilde_.subscribe("/odom", 2, new_odom_cb_);
  joy_sub_ = nh_tilde_.subscribe("/joy", 10, joy_cb_);

  joint_pub_ = nh_tilde_.advertise<sensor_msgs::JointState>("/command", 5);
  
  /* Load the robot model */
  robot_model_loader::RobotModelLoader robot_model_loader("robot_description"); 

  /* Get a shared pointer to the model */
  robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();

  /* Get and print the name of the coordinate frame in which the transforms for this model are computed*/
  ROS_INFO("Model frame: %s", kinematic_model->getModelFrame().c_str());  
  
  /* WORKING WITH THE KINEMATIC STATE */
  /* Create a kinematic state - this represents the configuration for the robot represented by kinematic_model */
  robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));

  /* Set all joints in this state to their default values */
  kinematic_state->setToDefaultValues();  

  /* Get the configuration for the joints in the right arm of the PR2*/
  robot_state::JointStateGroup* joint_state_group = kinematic_state->getJointStateGroup("manipulator");

  /* Get the names of the joints in the right_arm*/
  const std::vector<std::string> &joint_names = joint_state_group->getJointModelGroup()->getJointModelNames();
  
  /* Get the joint states for the right arm*/
  std::vector<double> joint_values;
  joint_state_group->getVariableValues(joint_values);

  /* Print joint names and values */
  //for(std::size_t i = 0; i < joint_names.size(); ++i)
  //{
  //  ROS_INFO("Joint %s: %f", joint_names[i].c_str(), joint_values[i]);
  //}

  /* Set one joint in the right arm outside its joint limit */
  //joint_values[0] = 1.57;
  //joint_state_group->setVariableValues(joint_values);  
  
  /* Check whether any joint is outside its joint limits */
  //ROS_INFO_STREAM("Current state is " << (kinematic_state->satisfiesBounds() ? "valid" : "not valid"));  

  /* Enforce the joint limits for this state and check again*/
  //kinematic_state->enforceBounds();
  //ROS_INFO_STREAM("Current state is " << (kinematic_state->satisfiesBounds() ? "valid" : "not valid"));    

  /* FORWARD KINEMATICS */
  /* Compute FK for a set of random joint values*/
  //joint_state_group->setToRandomValues();
  //const Eigen::Affine3d &end_effector_state = joint_state_group->getRobotState()->getLinkState("link_7")->getGlobalLinkTransform();        
  
  Eigen::Affine3d end_effector_state;

  /* Print end-effector pose. Remember that this is in the model frame */
  //ROS_INFO_STREAM("Translation: " << end_effector_state.translation()); 
  //ROS_INFO_STREAM("Rotation: " << end_effector_state.rotation()); 

  /* INVERSE KINEMATICS */
  /* Set joint state group to a set of random values*/
  //joint_state_group->setToRandomValues();

  /* Do IK on the pose we just generated using forward kinematics
   * Here 10 is the number of random restart and 0.1 is the allowed time after
   * each restart
   */

  std::vector<double> vel;
  double p[] = {0.2,0.2,0.2,0.2,0.2,0.2,0.2};
  std::vector<double> a(p, p+7);
  sensor_msgs::JointState msg; 
 
  while(nh_tilde_.ok())
  {
    end_effector_state = joint_state_group->getRobotState()->getLinkState("link_7")->getGlobalLinkTransform();

    tf::poseEigenToMsg(end_effector_state, actual_end_effector_); 

    //desired_end_effector_.position.x = 1.0 * actual_end_effector_.position.x * joy_data_.axes[1];
    //ROS_INFO_STREAM("Joy: " << joy_data_.axes[1]);

    //ROS_INFO_STREAM("X: " << desired_end_effector.position.x);

    //tf::poseMsgToEigen(tracked_object_.pose.pose, end_effector_state);
    bool found_ik = joint_state_group->setFromIK(end_effector_state, 10, 0.1);

    //ROS_INFO_STREAM("Position: " << actual_end_effector_.position.x);
    //ROS_INFO_STREAM("Position: " << tracked_object_.pose.pose.position);
    //ROS_INFO_STREAM("Translation: " << end_effector_state.translation()); 

    if (found_ik)
    {
      joint_state_group->getVariableValues(joint_values);
      msg.header.stamp = ros::Time::now();  
      msg.name = joint_names; 
      msg.position = joint_values;
      msg.velocity = a;
      //joint_pub_.publish(msg);
    }
    else
    {
      ROS_INFO("Did not find IK solution");
    }
  }

  ros::shutdown();
  return 0;
}
