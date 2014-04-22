#include <ros/ros.h>

// MoveIt!
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

#include <moveit/planning_interface/planning_interface.h>
#include <moveit/planning_scene/planning_scene.h>
#include <moveit/kinematic_constraints/utils.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/PlanningScene.h>

//#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <eigen_conversions/eigen_msg.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/PoseStamped.h>

class TeleopMK2
{
public:
  TeleopMK2();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  void poseCallback(const geometry_msgs::PoseStamped box);
  robot_model::RobotModelPtr kinematic_model;
  ros::NodeHandle nh_;

  ros::Subscriber joy_sub_;
  ros::Subscriber box_pub;
  ros::Publisher joint_pub_;

  geometry_msgs::Pose actual_end_effector_;
  geometry_msgs::Pose desired_end_effector_;
  geometry_msgs::PoseStamped temp_box_pose;
  geometry_msgs::PoseStamped box_pose;
};

TeleopMK2::TeleopMK2()
{
  joint_pub_ = nh_.advertise<sensor_msgs::JointState>("/command", 5);
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopMK2::joyCallback, this);
  box_pub = nh_.subscribe<geometry_msgs::PoseStamped>("/box_pose",10, &TeleopMK2::poseCallback, this);
  /* Load the robot model */
  robot_model_loader::RobotModelLoader robot_model_loader("robot_description"); 

  /* Get a shared pointer to the model */
  kinematic_model = robot_model_loader.getModel();

  /* Get and print the name of the coordinate frame in which the transforms for this model are computed */
  ROS_INFO("Model frame: %s", kinematic_model->getModelFrame().c_str());
  
  /* WORKING WITH THE KINEMATIC STATE */
  /* Create a kinematic state - this represents the configuration for the robot represented by kinematic_model */
  robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));

  /* Set all joints in this state to their default values */
  kinematic_state->setToDefaultValues();  

  /* Get the configuration for the joints in the right arm of the MK2 */
 const robot_state::JointModelGroup* joint_state_group = kinematic_state->getJointModelGroup("manipulator");//"manipulator");

  /* Get the names of the joints in the right_arm*/
  const std::vector<std::string> &joint_names = joint_state_group->getJointModelNames();
 for (int i=0; i<10; i++)
 { 
	std::cout << joint_names.at(i) << std::endl;
 }
// Get Joint Values
  // ^^^^^^^^^^^^^^^^
  /* Get the joint states for the right arm*/
  std::vector<double> joint_values;
  kinematic_state->copyJointGroupPositions(joint_state_group,joint_values);


  std::vector<double> vel;
  double p[] = {0.2,0.2,0.2,0.4,0.4,0.4,0.4,0.6,0.6,0.6};
  std::vector<double> a(p, p+10);
  sensor_msgs::JointState msg; 

  ros::Rate r(50);

  const Eigen::Affine3d &end_effector_state = kinematic_state->getGlobalLinkTransform("link_7");

  Eigen::Affine3d new_end_effector_state;
  tf::poseEigenToMsg(end_effector_state, actual_end_effector_);

  while(nh_.ok())
  {
    tf::poseMsgToEigen(actual_end_effector_, new_end_effector_state);

    bool found_ik = kinematic_state->setFromIK(joint_state_group, new_end_effector_state, 10, 0.1);

    if (found_ik)
    {
      kinematic_state->copyJointGroupPositions(joint_state_group,joint_values);
      msg.header.stamp = ros::Time::now();  
      msg.name = joint_names; 
      msg.position = joint_values;
      msg.velocity = a;
      joint_pub_.publish(msg);
    }
    else
    {
      ROS_INFO("Did not find IK solution");
    }

    ros::spinOnce();
    r.sleep();
  }
}

void TeleopMK2::poseCallback(const geometry_msgs::PoseStamped box)
{
	temp_box_pose = box;
}

void TeleopMK2::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{

  actual_end_effector_.orientation.w = 1.0;
//  actual_end_effector_.orientation.x = 0.0;
//  actual_end_effector_.orientation.y = 0.0;
//  actual_end_effector_.orientation.z = 0.0;


  if (joy->buttons[0])
  {
    //TeleopMK2::kinematic_state->setToDefaultValues();  
    actual_end_effector_.position.x = 0.29855;
    actual_end_effector_.position.y = -0.24952;
    actual_end_effector_.position.z = -0.88; 
  }

  else if (joy->buttons[10])
  {// 	pick up box UP pose  Pos: 0.29855  -0.0208635  -0.37089 | 0.29855  -0.0208635  -0.552698


    //actual_end_effector_.position.x = 0.3;
    //actual_end_effector_.position.y = 0.0;
    //actual_end_effector_.position.z = -0.88; 

  //  	actual_end_effector_.position.x = 0.29;
//	actual_end_effector_.position.y = -0.02;
//	actual_end_effector_.position.z = -0.42;

/* working offsets

x=-y+0.2
y=x-0.1
z=-z+0.1

*/
    actual_end_effector_.position.x = box_pose.pose.position.y+0.2;
    actual_end_effector_.position.y = box_pose.pose.position.x-0.1;
    actual_end_effector_.position.z = -box_pose.pose.position.z+0.1;
    actual_end_effector_.orientation.x = box_pose.pose.orientation.x;
    actual_end_effector_.orientation.y = box_pose.pose.orientation.y;
    actual_end_effector_.orientation.z = box_pose.pose.orientation.z;
  }

  else if (joy->buttons[8])
  {//	ABOVE ALL BOXES


    //actual_end_effector_.position.x = 0.3;
    //actual_end_effector_.position.y = -0.25;
    //actual_end_effector_.position.z = -0.75;
    	actual_end_effector_.position.x = 0.43;
	actual_end_effector_.position.y = -0.061;
	actual_end_effector_.position.z = -0.488;
  }

  else if (joy->buttons[6])
  {// DROP OFF LOCATION

    //actual_end_effector_.position.x = 0.3;
    //actual_end_effector_.position.y = -0.5;
    //actual_end_effector_.position.z = -0.88;
	actual_end_effector_.position.x = 0;
	actual_end_effector_.position.y = -0.65;
	actual_end_effector_.position.z = -0.78;
  }

  else if (joy->buttons[11])
  {///put down box UP pose : Pos: 0.285059  -0.506483  -0.451006

    actual_end_effector_.position.x =0.285;
    actual_end_effector_.position.y = -0.506;
    actual_end_effector_.position.z =-0.451;
  }

  else if (joy->buttons[9])
  {
	//button 10 saves pose
	box_pose = temp_box_pose;
  }

  else if (joy->buttons[7])
  {
    //actual_end_effector_.position.x =
    //actual_end_effector_.position.y = 
    //actual_end_effector_.position.z =
  }

  actual_end_effector_.position.x += 0.005 * joy->axes[1];
  actual_end_effector_.position.y += 0.005 * joy->axes[0];
  actual_end_effector_.position.z += 0.005 * joy->axes[2]; //-0.88 + 0.3 * joy->axes[3];
  ROS_INFO_STREAM ("Pos: " << actual_end_effector_.position.x << "  " << actual_end_effector_.position.y << "  " << actual_end_effector_.position.z);

  if (joy->buttons[0])
  {
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    kinematic_state->setToDefaultValues();  
  }
  
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_mk2");
  TeleopMK2 teleop_mk2;
}
