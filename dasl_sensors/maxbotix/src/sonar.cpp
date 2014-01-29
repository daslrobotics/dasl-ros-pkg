/* 
 * rosserial Ultrasound Example
 * 
 * This example is for the Maxbotix Ultrasound rangers.
 */

#define REPLY_SIZE 8
#define TIMEOUT 1000

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"

#include "cereal_port/CerealPort.h"
#include <sstream>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar");
  ros::NodeHandle n;
  ros::Publisher sonar_pub = n.advertise<sensor_msgs::Range>("sonar", 10);
  ros::Rate loop_rate(5);

  cereal::CerealPort device;
  char reply[REPLY_SIZE];
  
  try{ device.open("/dev/ttyUSB0", 57600); }
    catch(cereal::Exception& e)
    {
      ROS_FATAL("Failed to open the serial port!!!");
      ROS_BREAK();
    }
    ROS_INFO("The serial port is opened.");

  while (ros::ok())
  {
    try{ device.read(reply, REPLY_SIZE, TIMEOUT); }
      catch(cereal::TimeoutException& e)
      {
        ROS_ERROR("Timeout!");
      }

    std_msgs::String msg;
    std::istringstream iss(reply, std::istringstream::in);
    std::getline(iss, msg.data, ' ');

    ROS_INFO("%s", msg.data.c_str());

    ros::Time ping_time = ros::Time::now();
    sensor_msgs::Range ping;
    ping.header.stamp = ping_time;
    ping.range = atof(msg.data.c_str()) * 0.0254;

    sonar_pub.publish(ping);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
