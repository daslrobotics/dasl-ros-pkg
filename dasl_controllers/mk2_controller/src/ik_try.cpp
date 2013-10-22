#include <moveit/move_group_interface/move_group.h>
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Quaternion.h>
// MoveIt!
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/GetStateValidity.h>
#include <moveit_msgs/DisplayRobotState.h>

// IK 
#include <moveit_msgs/GetPositionIK.h>
#include <moveit/robot_state/joint_state_group.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/conversions.h>

// PI
#include <boost/math/constants/constants.hpp>

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

  /* Needed for ROS_INFO commands to work */
  ros::AsyncSpinner spinner(1);
  spinner.start();
  
  /* Load the robot model */
  //robot_model_loader::RobotModelLoader robot_model_loader("robot_description"); 

  /* Get a shared pointer to the model */
  //robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();

  /* Create a kinematic state - this represents the configuration for the robot represented by kinematic_model */
  //robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));

  /* Get the configuration for the joints in the right arm of the PR2*/
  //robot_state::JointStateGroup* joint_state_group = kinematic_state->getJointStateGroup("manipulator");

  /* PUBLISH RANDOM ARM POSITIONS */
  ros::NodeHandle nh;
  ros::Publisher robot_state_publisher = nh.advertise<moveit_msgs::DisplayRobotState>( "tutorial_robot_state", 1 );
  
  /* POSITION END EFFECTOR AT SPECIFIC LOCATIONS */

  /* Find the default pose for the end effector */
  //kinematic_state->setToDefaultValues();  

  //const Eigen::Affine3d end_effector_default_pose =
  //    kinematic_state->getLinkState("link_7")->getGlobalLinkTransform();        

  ros::Subscriber click=nh.subscribe("coor",10,track_hand);
  geometry_msgs::PoseStamped target;

  // start a ROS spinning thread
  //ros::AsyncSpinner spinner(1);
  //ros::NodeHandle nh;

  //spinner.start();

  robot_model_loader::RobotModelLoader robot_model_loader("robot_description"); 

  robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
  ROS_INFO("Model frame: %s", kinematic_model->getModelFrame().c_str());  

  robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
  kinematic_state->setToDefaultValues();  
  robot_state::JointStateGroup* joint_state_group = kinematic_state->getJointStateGroup("manipulator");  
  const std::vector<std::string> &joint_names=joint_state_group->getJointModelGroup()->getJointModelNames();

  const Eigen::Affine3d end_effector_default_pose =
      kinematic_state->getLinkState("link_7")->getGlobalLinkTransform();

/* Compute FK for a set of random joint values*/
  //joint_state_group->setToRandomValues();
  //const Eigen::Affine3d &end_effector_state = joint_state_group->getRobotState()->getLinkState("link_7")->getGlobalLinkTransform();  

//  Eigen::Affine3d end_effector_pose = 
  //  Eigen::Translation3d(RADIUS * cos(angle), RADIUS * sin(angle), 0.0) * end_effector_default_pose;

 // end_effector_state = [0,0,0,0,0,0,0];

  /* Get the joint values*/
 // std::vector<double> joint_values;
 // joint_state_group->getVariableValues(joint_values);

  //bool found_ik = joint_state_group->setFromIK(end_effector_state, 5, 0.1);

/*
if(found_ik)
{
  joint_state_group->getVariableValues(joint_values);
  for(std::size_t i=0; i <joint_names.size(); ++i)
  {
    ROS_INFO("Joint %s: %f", joint_names[i].c_str(), joint_values[i]);
  }
}
else
{
  ROS_INFO("Did not find IK solution");
}
*/

  ros::Rate rate(5.0);

  while(nh.ok())
  {

   /* calculate a position for the end effector */
//    Eigen::Affine3d end_effector_pose = 
  //    Eigen::Translation3d(xt, yt, zt) * end_effector_default_pose;

//    ROS_INFO_STREAM("End effector position: " << end_effector_pose.translation());
    ROS_INFO_STREAM("End effector default position: " << end_effector_default_pose.translation());
    ROS_INFO("Marker pose X:%.2f Y:%.2f Z:%.2f", xt, yt, zt);

    /* use IK to get joint angles satisfyuing the calculated position */
//    bool found_ik = joint_state_group->setFromIK(end_effector_pose, 10, 0.1);
  //  if (!found_ik)
 //   {
 //     ROS_INFO_STREAM("Could not solve IK for pose\n" << end_effector_pose.translation());
 //     continue;
 //   }
    
    /* get a robot state message describing the pose in kinematic_state */
//    moveit_msgs::DisplayRobotState msg;
//    robot_state::robotStateToRobotStateMsg(*kinematic_state, msg.state);

    /* send the message to the RobotState display */
//    robot_state_publisher.publish( msg );

    /* let ROS send the message, then wait a while */
//    ros::spinOnce();
//    ros::Duration(0.8).sleep();


  //service_request.ik_request.pose_stamped.pose.position.y = yt;
  //service_request.ik_request.pose_stamped.pose.position.z = zt;

  //service_client.call(service_request, service_response);
  //printf ("Y: %.2f, Z: %.2f\n", yt,zt);
  //else
    //printf ("Service call failed\n");


//ROS_INFO_STREAM("Result: " << ((service_response.error_code.val == service_response.error_code.SUCCESS) ? "True " : "False ") << service_response.error_code.val);

   //group.setPositionTarget (0.68,yt,zt,"link_7");
   //group.move();


  // printf ("POS[0]: %.2f\n", service_response.solution.joint_state.position[0]);

   //ros::Duration(0.4).sleep(); // sleep for 3 a second
   rate.sleep();
  }

  ros::shutdown();  
  return 0;
}

