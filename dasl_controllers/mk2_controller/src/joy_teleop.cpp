// MoveIt!
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_state/joint_state_group.h>

#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <eigen_conversions/eigen_msg.h>
#include <sensor_msgs/JointState.h>

class TeleopMK2
{
public:
  TeleopMK2();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  
  ros::NodeHandle nh_;

  ros::Subscriber joy_sub_;
  ros::Publisher joint_pub_;

  geometry_msgs::Pose actual_end_effector_;
  geometry_msgs::Pose desired_end_effector_;
};

TeleopMK2::TeleopMK2()
{
  joint_pub_ = nh_.advertise<sensor_msgs::JointState>("/command", 5);
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopMK2::joyCallback, this);

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

  /* Get the configuration for the joints in the right arm of the MK2*/
  robot_state::JointStateGroup* joint_state_group = kinematic_state->getJointStateGroup("manipulator");

  /* Get the names of the joints in the right_arm*/
  const std::vector<std::string> &joint_names = joint_state_group->getJointModelGroup()->getJointModelNames();
  
  /* Get the joint states for the right arm*/
  std::vector<double> joint_values;
  joint_state_group->getVariableValues(joint_values);

  Eigen::Affine3d end_effector_state;
  Eigen::Affine3d new_end_effector_state;

  std::vector<double> vel;
  double p[] = {0.5,0.5,0.5,0.5,0.5,0.5,0.5};
  std::vector<double> a(p, p+7);
  sensor_msgs::JointState msg; 

  ros::Rate r(50);

  end_effector_state = joint_state_group->getRobotState()->getLinkState("link_7")->getGlobalLinkTransform();

  tf::poseEigenToMsg(end_effector_state, actual_end_effector_);

  while(nh_.ok())
  {
    tf::poseMsgToEigen(actual_end_effector_, new_end_effector_state);

    bool found_ik = joint_state_group->setFromIK(new_end_effector_state, 10, 0.1);

    if (found_ik)
    {
      joint_state_group->getVariableValues(joint_values);
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

void TeleopMK2::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  actual_end_effector_.position.x += 0.01 * joy->axes[0];
  actual_end_effector_.position.z += 0.01 * joy->axes[1];

  actual_end_effector_.orientation.w = 1.0;
  actual_end_effector_.orientation.x = 0.0;
  actual_end_effector_.orientation.y = 0.0;
  actual_end_effector_.orientation.z = 0.0;

  ROS_INFO_STREAM ("Pos: " << actual_end_effector_.position.x << "  " << actual_end_effector_.position.z);

  //desired_end_effector_.position.x = actual_end_effector_.position.x + joy->axes[0];
  //desired_end_effector_.position.z = actual_end_effector_.position.z + joy->axes[1];
  //ROS_INFO_STREAM ("Pos: " << desired_end_effector_.position.x);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_mk2");
  TeleopMK2 teleop_mk2;
  //ros::AsyncSpinner spinner(1);
  //spinner.start();
  //ros::spin();
}
