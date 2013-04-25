/*!
 *****************************************************************
 * \file
 *
 * \note
 *   Copyright (c) 2013 \n
 *   Drexel University
 *
 *****************************************************************
 *
 * \note
 *   Project name: mk2_controller
 * \note
 *   ROS package name: dasl-ros-pkg
 *
 * \author
 *   Author: Christopher Korpela email: christopher.korpela@gmail.com
 * \author
 *   Supervised by: Dr. Paul Oh
 *
 * \date Date of creation: Jan 2013
 *
 * \brief
 *   Implementation of ROS node for MK2 arm.
 *
 *****************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer. \n
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution. \n
 *     - Neither the name of the Fraunhofer Institute for Manufacturing
 *       Engineering and Automation (IPA) nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission. \n
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License LGPL as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License LGPL for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License LGPL along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************/

//##################
//#### includes ####
//##################

// Standard includes
#include <pthread.h> 

// ROS includes
#include <ros/ros.h>
#include <urdf/model.h>

// ROS message includes
#include <std_msgs/Float64.h>
#include <ros/console.h>
#include <sensor_msgs/JointState.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <pr2_controllers_msgs/JointTrajectoryControllerState.h>
#include <diagnostic_msgs/DiagnosticArray.h>

#include <std_msgs/String.h>


// HDT MK2 includes
#include "stdafx.h"
#include "messages.h"
#include "gateway.h"
#include "can_ifc.h"
#include "controller.h"

// Type definitions - convert Windows-specific variables
typedef unsigned int DWORD;  
typedef struct sockaddr * LPSOCKADDR;
typedef void * LPVOID;

MK2::MK2Drive Drives[64];

pthread_t recvThreadID;	

int clientSock = -1; 
struct sockaddr_in clientAddr; 
struct sockaddr_in serverAddr; /*server address*/

bool req_ls_tlm[64];
bool req_ms_tlm[64];
bool req_hs_tlm[64];
bool wait_some_more = false;
 
void init_udp(void)
{
	int s;			
	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)	
	{
		ROS_ERROR("Startup Failed");
		exit(1);
	}
	 
	memset((char*)&clientAddr,0,sizeof(clientAddr));
	clientAddr.sin_family=AF_INET; /*set client address protocol family*/
	clientAddr.sin_addr.s_addr=INADDR_ANY;
	clientAddr.sin_port=htons((u_short)GATEWAY_IP_PORT_NUM);  

	serverAddr.sin_addr.s_addr=inet_addr(GATEWAY_IP_ADDRESS_STRING); 
    	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons((u_short)GATEWAY_IP_PORT_NUM);

	if (serverAddr.sin_addr.s_addr==INADDR_NONE)
	{
		ROS_ERROR("Unable to obtain Gateway IP (bad IP)");
		//system("PAUSE");
		exit(1);
    }
	  
	clientSock = socket(PF_INET,SOCK_DGRAM,0);/*create a socket*/

	if (clientSock<0)
	{
		ROS_ERROR("Socket creation failed\n");
		//system("PAUSE");
		exit(1);
	}

	/*bind a client address and port*/
	if (bind(clientSock,(LPSOCKADDR)&clientAddr,sizeof(struct sockaddr))<0)
	{
		ROS_ERROR("Socket bind failed\n");
		//system("PAUSE");
		exit(1);
	}
}

  /*!
   * \brief Callback to send messages
   */
void send_via_udp(MK2::Message *msg)
{
	uint8_t udp_data[512];
	uint8_t udp_data_idx = 0;
	MK2::CanPacket OutPackets[32];

	//convert the message into can packets suitable for CAN bus
	uint16_t packets = MK2::convertMsg2Can(OutPackets, 32, msg);

	if(packets > 1 || packets <= 0) return; //we can handle more, but we should only be using messages that are 1 can packet

	//convert the can packets into a udp message
	for(int i = 0; i < packets; i++)
	{		
		if (Drives[msg->destination].port == 3)
		{
			//send to both ports
			OutPackets[i].port = 0;
			MK2::Can2Gateway(&udp_data[(udp_data_idx++)*16], 16, &OutPackets[i]); //right port of the gateway is connected to the outside.
			OutPackets[i].port = 1;
			MK2::Can2Gateway(&udp_data[(udp_data_idx++)*16], 16, &OutPackets[i]); //right port of the gateway is connected to the outside.
		}else

		{
			OutPackets[i].port = Drives[msg->destination].port & 0x01;
			MK2::Can2Gateway(&udp_data[(udp_data_idx++)*16], 16, &OutPackets[i]); //right port of the gateway is connected to the outside.
		}
	}

	uint16_t len = udp_data_idx * 16;

	//the gateway can only handle 32 can packets per udp message, anymore and a buffer mechanism is needed
	sendto(clientSock, (char *)udp_data,len,0,(LPSOCKADDR)&serverAddr,sizeof(serverAddr));
}

  /*!
   * \brief Thread to read input from gateway
   */
void *receive_via_udp(void *ptr)
{
	char udp_data[512];
	int recv_len = 0;
	MK2::CanPacket can_packet;
	MK2::Message msg;
	MK2::MK2Drive *drive;
	struct sockaddr_in recvAddr;
	
	recvAddr.sin_addr.s_addr=INADDR_ANY;
	recvAddr.sin_port = htons(0);
	recvAddr.sin_family = AF_INET;
	socklen_t recvAdddr_len = sizeof(recvAddr);

	while(1)
	{
		recv_len = recvfrom(clientSock,udp_data,512,0,(LPSOCKADDR)&recvAddr, &recvAdddr_len);

		if(recv_len == -1)
		{
			ROS_ERROR("Invalid length");
		}
	if(recv_len % 16 != 0) continue; //must be multiples of 16 to be valid

	for(int i = 0; i < recv_len/16; i++)
	{		 
		MK2::Gateway2Can(&can_packet, (uint8_t *) &udp_data[16 * i], 16);

		if(MK2::convertCan2Msg(&msg, &can_packet) <= 0) continue;
		if(msg.source >= 64) continue;

		drive = &Drives[msg.source];
		
		drive->port = can_packet.port;

		switch(msg.message_id)
		{
			case MK2::MessageNumberEnum::HIGH_SPEED_TLM:
				if(MK2::process_hs_telemetry(&drive->hs, &msg) && req_hs_tlm[msg.source])
				{
					//ROS_INFO("%s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					//ROS_INFO("POS:%.2f VEL:%.2f TRQ:%.2f I:%.2f L:%d", drive->hs.position, drive->hs.velocity, drive->hs.torque, drive->hs.inst_current, drive->hs.position_limit);	
					req_hs_tlm[msg.source] = false;
				}
				break;

			case MK2::MessageNumberEnum::MED_SPEED_TLM:
				if(MK2::process_ms_telemetry(&drive->ms, &msg) && req_ms_tlm[msg.source])
				{
					//ROS_INFO("%s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					//ROS_INFO("V1:%d V2:%d V3:%d V4:%d",  drive->ms.var1, drive->ms.var2, drive->ms.var3, drive->ms.var4);	
					req_ms_tlm[msg.source] = false;
				}
				break;

			case MK2::MessageNumberEnum::LOW_SPEED_TLM:
				if(MK2::process_ls_telemetry(&drive->ls, &msg) && req_ls_tlm[msg.source])
				{
					//ROS_INFO("%s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					//ROS_INFO("SW:%d FOC:%d V:%dV I:%dA T:%dC",  drive->ls.sw_state, drive->ls.foc_state, (int)drive->ls.bus_voltage, (int)drive->ls.bus_current, (int)drive->ls.temperature);
				req_ls_tlm[msg.source] = false;
				}
				break;

			case MK2::MessageNumberEnum::BIT_ERR_TLM:

				if(MK2::process_bit_response(&drive->bit, &msg))
				{
					//ROS_INFO("%s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					drive->status.sw_state = MK2::SWStates::FS; //should send status request, but state is already FS due to BIT
					drive->ls.sw_state = MK2::SWStates::FS;
					//ROS_INFO("STATE:%s", MK2::GetSwStateString(drive->status.sw_state));					
					wait_some_more = true;
				}
				break;

			case MK2::MessageNumberEnum::STATUS_TLM:
				if(MK2::process_status_response(&drive->status, &msg))
				{
					//ROS_INFO("%s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					//ROS_INFO("STATE:%s", MK2::GetSwStateString(drive->status.sw_state));
					wait_some_more = true;
				}
				break;
			case MK2::MessageNumberEnum::CMD_ACK_TLM:
				if(MK2::process_cmd_ack(&drive->ack, &msg))
				{
					//ROS_INFO("%s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					//ROS_INFO("MSG:%s", MK2::GetMessageNameString(drive->ack.msg_id));
					if(drive->ack.msg_id == MK2::MessageNumberEnum::CHANGE_STATE_CMD) //see what the new state is
					{
						MK2::send_status_req(msg.source);
					}
					wait_some_more = true;
				}
				break;
			case MK2::MessageNumberEnum::CMD_NACK_TLM:
				if(MK2::process_cmd_nack(&drive->nack, &msg))
				{
					//ROS_INFO("%s ID:%d  ",MK2::GetMessageNameString(msg.message_id), msg.source);
					//ROS_INFO("MSG:%s ERROR:%x",  MK2::GetMessageNameString(drive->nack.msg_id), drive->nack.error_code);
					wait_some_more = true;
				}
				break;
		}
	}
	}

	return 0;
}

/* ******************************************************** */

namespace MK2
{

/* ******************************************************** */
MK2Driver::MK2Driver()
{
  // For normal drivers, you would read custom configuration settings from the parameter
  // server here. Common examples include the port name over which you are communicating,
  // or the baud rate for the serial port. For this simple driver, no additional information
  // is required.

  // For example purposes, we are going to call the base class methods manually.

  // Create a private node handle to read parameter settings
  ros::NodeHandle node = ros::NodeHandle();
  ros::NodeHandle private_node = ros::NodeHandle("~");

  // Get the robot_description parameter name from the parameter server
  private_node.param("robot_description_parameter", robot_description_parameter_, std::string("robot_description"));

  // Read in additional joint information from the robot description and store it in the urdf model
  ActuatorArrayDriver::parse_urdf(node);

  // Fill in the joints_ map from the 'joints' list on the parameter server
  ActuatorArrayDriver::parse_actuator_list(private_node);

  // Advertise services and subscribe to topics
  ActuatorArrayDriver::advertise_and_subscribe(node);
}

/* ******************************************************** */
MK2Driver::~MK2Driver()
{
}

/* ******************************************************** */
bool MK2Driver::init_actuator_(const std::string& joint_name, MK2JointProperties& joint_properties, XmlRpc::XmlRpcValue& joint_data)
{
  // Read the additional actuator fields of 'channel' and 'home'
  // from the configuration file data, then create and store a dummy_servo
  // with those parameters

  // Read custom data from the XMLRPC struct
  if (joint_data.hasMember("channel"))
  {
    joint_properties.channel = (int) joint_data["channel"];
  }

  if (joint_data.hasMember("home"))
  {
    joint_properties.home = (double) joint_data["home"];
  }

  // Create an actuator object and store in a container
  actuators_[joint_properties.channel] = MK2Actuator(joint_properties.min_position, joint_properties.max_position, joint_properties.max_velocity, joint_properties.home);

  return true;
}

/* ******************************************************** */
bool MK2Driver::read_(ros::Time ts)
{
  // Calculate the time from the last update
  double dt = (ts - previous_time_).toSec();

  // Loop through each joint and request the current status
  for (unsigned int i = 0; i < joint_state_msg_.name.size(); ++i)
  {
    // Look up the channel number from the JointProperties using the joint name
    int channel = joints_[command_msg_.name[i]].channel;

    // Update the simulated state of each actuator by dt seconds
    actuators_[channel].update(dt);

    // Query the current state of each actuator
    //joint_state_msg_.position[i] = actuators_[channel].getPosition();
    //joint_state_msg_.velocity[i] = actuators_[channel].getVelocity();
    //joint_state_msg_.effort[i]   = actuators_[channel].getMaxTorque();

    joint_state_msg_.position[i] = Drives[channel].hs.position;
    joint_state_msg_.velocity[i] = Drives[channel].hs.velocity;
    joint_state_msg_.effort[i] = Drives[channel].hs.torque;
  }

  joint_state_msg_.header.stamp = ts;
  previous_time_ = ts;

  return true;
}

/* ******************************************************** */
bool MK2Driver::command_()
{
  // Loop through each joint in the command message and send the
  // corresponding servo the desired behavior
  for (unsigned int i = 0; i < command_msg_.name.size(); ++i)
  {
    // Look up the channel number from the JointProperties using the joint name
    int channel = joints_[command_msg_.name[i]].channel;

    // Send the actuator the desired position and velocity
    //actuators_[channel].setVelocity(command_msg_.velocity[i]);
    //actuators_[channel].setPosition(command_msg_.position[i]);

    Drives[channel].control.velocity = command_msg_.velocity[i];
    Drives[channel].control.position = command_msg_.position[i];
    Drives[channel].control.torque = command_msg_.effort[i];
  }

  return true;
}

/* ******************************************************** */
bool MK2Driver::stop_()
{
  // Loop through each joint and send the stop command
  for (unsigned int i = 0; i < command_msg_.name.size(); ++i)
  {
    // Look up the channel number from the JointProperties using the joint name
    int channel = joints_[command_msg_.name[i]].channel;

    // Tell the actuator to stop
    //actuators_[channel].stop();

	Drives[channel].control.inst_current_limit = 0;
	Drives[channel].control.velocity = 0;
	Drives[channel].control_enable = false;
  }

  return true;
}

/* ******************************************************** */
bool MK2Driver::home_()
{
  // Loop through each joint and send the stop command
  for (unsigned int i = 0; i < command_msg_.name.size(); ++i)
  {
    // Look up the channel number from the JointProperties using the joint name
    int channel = joints_[command_msg_.name[i]].channel;

    // Tell the actuator to go to the home position
    //actuators_[channel].home();

    if (channel == 2)
      Drives[channel].control.position = 0;
    else
      Drives[channel].control.position = 0;

    if ((channel >= 49 && channel <= 52) || (channel >= 10 && channel <= 13))
      Drives[channel].control.velocity = 0.2;
    else
      Drives[channel].control.velocity = 0.2;
  }

  return true;
}

/* ******************************************************** */

} // end MK2 namespace

/* ******************************************************** */
void control_cmd_callback(const ros::TimerEvent& event)
{
	MK2::MK2Drive *drive;

	for(int i = 0 ; i < 64; i++)
	{
		drive = &Drives[i];
		if(drive->control_enable == true)
		{
			if(drive->status.sw_state == MK2::SWStates::NOS_CONTROL || 
			   drive->status.sw_state == MK2::SWStates::NOS_IDLE || 
			   drive->status.sw_state == MK2::SWStates::NOS_SLEEP)
			{
				MK2::send_control_cmd(i, &drive->control);
			}
		}
	}
}

/* ******************************************************** */
void status_callback(const ros::TimerEvent& event)
{
    MK2::send_status_req(MK2::NodeID::ALL_DEVICES);

    // Use this area for periodic display of information

    //ROS_INFO("Rigth shoulder roll position: %f", Drives[2].hs.position);

    //ROS_INFO("Torso roll torque: %f", Drives[56].hs.torque);
    //ROS_INFO("Torso pitch torque: %f", Drives[16].hs.torque);

    //ROS_INFO("Thumb yaw torque: %f", Drives[49].hs.torque);
    //ROS_INFO("Thumb pitch torque: %f", Drives[50].hs.torque);
    //ROS_INFO("Index finger torque: %f", Drives[51].hs.torque);
    //ROS_INFO("Middle finger torque: %f", Drives[52].hs.torque);
}

/* ******************************************************** */
void joint_path_callback(const std_msgs::StringConstPtr& str)
{

	ROS_INFO("^^^^^^^^^^^^^^^^^: %s", str->data.c_str());

}

/* ******************************************************** */
void req_all_tlm_(bool req[])
{
	for(int i = 0; i < 64; i++)
	{
		req[i] = true;
	}
}

/* ******************************************************** */
void set_reset_status_to_all_drives_(void)
{
	for(int i = 0; i < 64; i++)
	{
		Drives[i].status.sw_state = MK2::SWStates::UNKNOWN;
		Drives[i].ls.sw_state = MK2::SWStates::UNKNOWN;
		Drives[i].control_enable = false;
		req_hs_tlm[i] = false;
		req_ls_tlm[i] = false;
		req_ms_tlm[i] = false;
	}
}

/* ******************************************************** */
int main(int argc, char** argv)
{
	// Initialize ROS node
	ros::init(argc, argv, "mk2_controller");
	ros::Time::init();
	ros::NodeHandle nh;
	
	// Initialize udp socket to talk to the gateway
	init_udp();

	// Set callback to send commands over UDP
	MK2::set_source_id(9);			//this id is sent to all drives, 9 is typically chosen as the controller
	MK2::set_output_cb(send_via_udp);	//all the send commands call the send_via_udp callback to output to udp

	// Create thread to receive UDP messages
	pthread_create(&recvThreadID, NULL, receive_via_udp, NULL);

	// Reset MK2 driver
	set_reset_status_to_all_drives_();		//reset values in structures that hold drive data	
	MK2::send_reset_cmd(MK2::NodeID::ALL_DEVICES);  //reset drive to a known state

	//a little hack for the send status request to send on both ports
	for(int i = 0; i < 64; i++)
	{
		Drives[i].port = 1;
	}
	Drives[15].port = 3;
	Drives[14].port = 3;
	Drives[16].port = 0;
	Drives[17].port = 0;

	ros::Duration(0.1).sleep();

	// Allow the console output to settle down
	while(wait_some_more) 
	{
		wait_some_more = false;
		ros::Duration(0.1).sleep();	
	}

	// Check all drives and change to NOS_CONTROL
	MK2::send_status_req(MK2::NodeID::ALL_DEVICES); //request all devices to report status		
	ros::Duration(0.3).sleep();

	for(int i = 0; i < 64; i++)
	{
		if(Drives[i].status.sw_state == MK2::SWStates::INIT)
		{
			MK2::send_change_state_cmd(i, MK2::SWStates::NOS_CONTROL);

			while ((Drives[i].status.sw_state != MK2::SWStates::NOS_CONTROL) &&
		   	       (Drives[i].status.sw_state != MK2::SWStates::NOS_IDLE) &&  
                   (Drives[i].status.sw_state != MK2::SWStates::NOS_SLEEP))
			{
				//ROS_INFO("Waiting for NOS Status: ID:%d  ", i);
				//ROS_INFO("STATE:%s", MK2::GetSwStateString(Drives[i].status.sw_state));					
				ros::Duration(0.1).sleep();
			}
			Drives[i].control.inst_current_limit = 0;
			Drives[i].control.velocity = 0;
			Drives[i].control_enable = false;
		}
	}

	// Cycle through all drives and set to NOS control
	// Do not enable drive 17

	for(int i = 0; i < 64; i++)
	{
	    if ((i >= 49 && i <= 52) || (i >= 10 && i <= 13))
			Drives[i].control.inst_current_limit = 3;
		else
			Drives[i].control.inst_current_limit = 8;
		if (i != 17) Drives[i].control_enable = true;
	}

    // ------------------------------------ IMPEDANCE TEST BED -------------------------
    /*

    // Enable impedance control on shoulder pitch joint (41) for testing

    // Elbow pitch


    int i=44;

    Drives[i].impedance.inertia = 1;//1;
    Drives[i].impedance.damping = 3;//3;
    Drives[i].impedance.stiffness = 30;//20;

	MK2::send_impedance_cmd(i, &Drives[i].impedance);

    // Wirst pitch

    i=46;

    Drives[i].impedance.inertia = 1;//1;
    Drives[i].impedance.damping = 3;//3;
    Drives[i].impedance.stiffness = 10;//20;

	MK2::send_impedance_cmd(i, &Drives[i].impedance);

    */
    // ------------------------------------ IMPEDANCE TEST BED ------------------------- END

    // Create controller using MK2Controller class
    MK2::MK2Driver controller;

    ros::Timer timer1 = nh.createTimer(ros::Duration(0.02), control_cmd_callback);
    ros::Timer timer2 = nh.createTimer(ros::Duration(0.5), status_callback);

//ROS_INFO("^^^^^^^^^^^^^^^^^:1");
    //ros::Subscriber sub = nh.subscribe("joint_path_command", 1, joint_path_callback);
//ROS_INFO("^^^^^^^^^^^^^^^^^:2");
    //ros::Subscriber sub_trajectory_state_ = nh.subscribe("feedback_states", 1, &JointTrajectoryAction::controllerStateCB, this);
  
    //joint_path subscriver callback here

    controller.spin();

    ros::waitForShutdown();

    close(clientSock);

    return 0;
}
