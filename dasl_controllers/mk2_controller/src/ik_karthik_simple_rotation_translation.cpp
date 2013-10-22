#include <ros/ros.h>

// MoveIt!
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/joint_state_group.h>
  
int main(int argc, char **argv)
{
  ros::init (argc, argv, "right_arm_kinematics");
  ros::AsyncSpinner spinner(1);
  spinner.start();
  
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
  for(std::size_t i = 0; i < joint_names.size(); ++i)
  {
    ROS_INFO("Joint %s: %f", joint_names[i].c_str(), joint_values[i]);
  }

  /* Set one joint in the right arm outside its joint limit */
  joint_values[0] = 1.57;
  joint_state_group->setVariableValues(joint_values);  
  
  /* Check whether any joint is outside its joint limits */
  ROS_INFO_STREAM("Current state is " << (kinematic_state->satisfiesBounds() ? "valid" : "not valid"));  

  /* Enforce the joint limits for this state and check again*/
  kinematic_state->enforceBounds();
  ROS_INFO_STREAM("Current state is " << (kinematic_state->satisfiesBounds() ? "valid" : "not valid"));    

  /* FORWARD KINEMATICS */
  /* Compute FK for a set of random joint values*/
  joint_state_group->setToRandomValues();
  const Eigen::Affine3d &end_effector_state = joint_state_group->getRobotState()->getLinkState("link_7")->getGlobalLinkTransform();        
  
  /* Print end-effector pose. Remember that this is in the model frame */
  ROS_INFO_STREAM("Translation: " << end_effector_state.translation()); 
  ROS_INFO_STREAM("Rotation: " << end_effector_state.rotation()); 

  /* INVERSE KINEMATICS */
  /* Set joint state group to a set of random values*/
  joint_state_group->setToRandomValues();

  /* Do IK on the pose we just generated using forward kinematics
   * Here 10 is the number of random restart and 0.1 is the allowed time after
   * each restart
   */
  bool found_ik = joint_state_group->setFromIK(end_effector_state, 10, 0.1);
  
  /* Get and print the joint values */
  if (found_ik)
  {
ROS_INFO("IK FOUND!!");
    joint_state_group->getVariableValues(joint_values);
    for(std::size_t i=0; i < joint_names.size(); ++i)
    {
      ROS_INFO("Joint %s: %f", joint_names[i].c_str(), joint_values[i]);
    }
  }
  else
  {
    ROS_INFO("Did not find IK solution");
  }

  

  ros::shutdown();  
  return 0;
}
