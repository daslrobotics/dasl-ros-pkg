#include "ros/ros.h"
#include "mocap/RigidBody.h"
#include <geometry_msgs/Transform.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Quaternion.h>
#include "mocap/pc2quadcopter.h"
#include "MOCAPSocket.h"
#include <sstream>
/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <arpa/inet.h>
#include "SDLinput.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
/*
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
	//
  /*
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "mocap");
  /*
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  //ros::Publisher chatter_pub = n.advertise<geometry_msgs::Transform>("servo", 1000);
  //ros::Publisher chatter_pub = n.advertise<mocap::pc2quadcopter>("pc2quadcopter", 1000);
  //ros::Rate loop_rate(100);

  //ros::init(argc, argv, "mocap", ros::init_options::NoSigintHandler );

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  //Joystick joy;
///////////////////////////////////////////////////////////////////////////
  MOCAPSocket Socket;
///////////////////////////////////////////////////////////////////////////

  static tf::TransformBroadcaster br;

  int count = 0;
  //float thrust=1000;
  //float roll=0.0;
  //float pitch=0.0;

  while (ros::ok())
  {
	  //mocap::pc2quadcopter msg;
   if(Socket.Read()>0)
   {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
	ros::Time timestamp( ros::Time::now() );

	tf::Transform transform;
	// Translate mocap data from mm --> m to be compatible with rviz
	transform.setOrigin( tf::Vector3(Socket.rigidBody[0].x / 1000.0f,Socket.rigidBody[0].z / 1000.0f,Socket.rigidBody[0].y / 1000.0f ) );
			    
	// Switch y and z axes due to calibration of mocap

	tf::Quaternion q( Socket.rigidBody[0].qx, Socket.rigidBody[0].qz, Socket.rigidBody[0].qy, Socket.rigidBody[0].qw ) ;
			    
	transform.setRotation(q.inverse());		// Handle 

	int rigid_body_id = abs(Socket.rigidBody[0].ID);
	const char* rigid_body_name = "OBJECT"; //mocap_model[rigid_body_id];
	br.sendTransform(tf::StampedTransform(transform, timestamp, "base_link", std::string( rigid_body_name ) ));

	//tf::TransformBroadcaster msg;
	
	//mocap::pc2quadcopter msg;

	//msg.orientation.x=Socket.rigidBody[0].roll*3.14/180;
	//msg.orientation.y=Socket.rigidBody[0].pitch*3.14/180;
	//msg.orientation.z=Socket.rigidBody[0].yaw*3.14/180;

	//msg.position.x= -Socket.rigidBody[0].z;
	//msg.position.y= Socket.rigidBody[0].x;
	//msg.position.z=Socket.rigidBody[0].y;

	//msg.speed.x=-(Socket.rigidBody[0].z-Socket.rigidBody[0].z_old)/20;
	//msg.speed.y=(Socket.rigidBody[0].x-Socket.rigidBody[0].x_old)/20;
	//msg.speed.z=(Socket.rigidBody[0].y-Socket.rigidBody[0].y_old)/20;

	//msg.pitch=pitch;
	//msg.roll=roll;
    	//chatter_pub.publish(msg);
    }
    //ROS_INFO("%s", msg.data.c_str());

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */

    ros::spinOnce();

    //loop_rate.sleep();
    ++count;
  }

  return 0;
}

