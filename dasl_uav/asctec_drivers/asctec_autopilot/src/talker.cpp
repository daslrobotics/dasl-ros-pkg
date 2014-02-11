#include "asctec_proc/asctec_proc.h"

int main(int argc, char **argv)
{

	ros::init(argc, argv, "talker");
	ros::NodeHandle n;	

	ros::Publisher thrust_pub = n.advertise<asctec_msgs::CtrlInput>("/asctec/CTRL_INPUT", 1);

	ros::Rate loop_rate(30);

while (ros::ok())
  {
	  asctec_msgs::CtrlInput msg;
	  msg.pitch = 0;
	ROS_INFO("pitch", msg.pitch);
	  msg.roll = 0;
ROS_INFO("roll", msg.roll);
	  msg.thrust = 0;
ROS_INFO("thrust", msg.thrust);
	  msg.yaw = -2048;
ROS_INFO("yaw", msg.yaw);
	  msg.ctrl = 0xF;
ROS_INFO("ctrl", msg.ctrl);
	  msg.chksum = msg.pitch
		     + msg.roll + msg.yaw
		     + msg.thrust + msg.ctrl + (short)0xAAAA;
ROS_INFO("chksum", msg.chksum);
	  thrust_pub.publish(msg);

	ros::spinOnce();

	loop_rate.sleep();

  }
return 0;
} 
