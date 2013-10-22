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

if(t==0)
{
//record the new position
xb=xt;
yb=yt;
zb=zt;
t=1;
}
else
{
std::cout<<"ye:"<<ye<<"\n";
std::cout<<"ze:"<<ze<<"\n";
  xe=xt-xb;//calculate the error from the previous position
  ye=yt-yb;
  ze=zt-zb;
  xb=xt;
  yb=yt;
  zb=zt;
}




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
if(t1==0)
{
y_new=-0.73;
z_new=1.28;
t1=1;
}
else
{
y_new=y_new+ye;
z_new=z_new+ze;
}


   group.setPositionTarget(0.702,y_new,z_new,"link_7");
// group.setPositionTarget(a,b,c,"link_name")
//a-forward move
//b-lateral movement
//c-up and down(altitude)

 a=group.move();
std::cout<<"!!"<<a<<"\n";

 // ros::Duration(1).sleep(); // sleep for 3 a second
}





  ros::waitForShutdown();
}



