#!/usr/bin/env python

PKG = 'gantry_controller'

import roslib; roslib.load_manifest(PKG)

import time
from threading import Thread
import math
from pid import PID
from UAV_velocity import UAV_velocity

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import JointState
from nav_msgs.msg import Odometry
import tf

class MoveGantryPID():
    def __init__(self):

        self.is_running = True
        self.step_size = 0.1
	self.torque_gain = 0.5
	self.gantry_cmd_vel = Twist()
        self.joint_states = None
        self.joy_data = None
	self.odom_data = None
        self.torso_pitch_commanded = 0
        self.dynamic_torque_pitch = 0

                             # P  MOI init speed max min
	self.UAV=UAV_velocity(0.1,150,0,0.5,-0.5)

        rospy.init_node('move_gantry_x_axis', anonymous=True)
        rospy.Subscriber('joy', Joy, self.read_joystick_data)
        rospy.Subscriber('joint_states', JointState, self.joint_state_handler)
        rospy.Subscriber('odom', Odometry, self.read_odom_data)
        self.joint_states_pub = rospy.Publisher('command', JointState)
	self.gantry_cmd_vel_pub = rospy.Publisher('cmd_vel', Twist)

    def joint_state_handler(self, msg):
	self.joint_states = msg

    def read_joystick_data(self, msg):
        self.joy_data = msg

    def read_odom_data(self, msg):
        self.odom_data = msg

    def update_gantry_velocity(self):

	# Wait for mocap, tf, and odom to become available
        rospy.sleep(3)

        print "Ready"

        # Intialize desired position with first odom position
	#self.x_pos_des = self.odom_data.pose.pose.position.x

        # Get static torque values
        self.static_torque_pitch = self.joint_states.effort[1]

        #Set control loop rate (Hz)
        r = rospy.Rate(10)

        #Set pitch torque deadband
        dead_band = 5

        while self.is_running:

            # Message for torso command
            msg = JointState()
            msg.name.append('torso_pitch_joint')
            msg.effort.append(0.0)

            # Get setpoint from joystick
            if self.joy_data:
                #if not self.joy_data.buttons[0]:
                    #self.gantry_cmd_vel.linear.x = -1 * self.joy_data.axes[1]
		self.UAV.set_point = -1 * self.joy_data.axes[1] * self.step_size

            # Introduce dynamic torque as a disturbance
	    if self.joint_states:
                # Calculate dynamic torques
		self.dynamic_torque_pitch = self.joint_states.effort[1] - self.static_torque_pitch 

            if abs(self.dynamic_torque_pitch) < dead_band:
                self.dynamic_torque_pitch = 0 
            else:
                if self.dynamic_torque_pitch > 0:
                    self.dynamic_torque_pitch -= dead_band / 2
                else:
                    self.dynamic_torque_pitch += dead_band / 2

	    self.gantry_cmd_vel.linear.x = self.UAV.update(self.odom_data.twist.twist.linear.x, -self.joint_states.position[1], -self.dynamic_torque_pitch)

	    self.torso_pitch_commanded = self.UAV.getPitchCmnd() * -2.5

                #print "Pitch: %.5f Gantry XVel: %0.5f" % (self.torso_pitch_commanded, self.odom_data.twist.twist.linear.x)

            msg.position.append(self.torso_pitch_commanded)
                #msg.position.append(0.0)
            msg.velocity.append(0.4)
            msg.header.stamp = rospy.Time.now()
            self.joint_states_pub.publish(msg)

                #-1 * self.joy_data.axes[1] * self.step_size
               
                #if self.joy_data.buttons[10]:
                #    #Send torso pitch home
                #    msg.position.append(0.0)
                #    msg.velocity.append(0.2)
	        #    msg.header.stamp = rospy.Time.now()
                #    self.joint_states_pub.publish(msg)
		
            # Introduce dynamic torque as a disturbance
	    #if self.joint_states:
                # Calculate dynamic torques
		#self.dynamic_torque_pitch = self.joint_states.effort[1] - self.static_torque_pitch 
             
            # Print Feedback 
            self.pitch_angle_actual = self.joint_states.position[1]
            #print "X pos: %.5f  X vel: %.5f" % (self.odom_data.pose.pose.position.x, self.odom_data.twist.twist.linear.x)
	    #print "X des: %.5f  Y des: %.5f  Z des: %.5f" % (self.x_pos_des, self.y_pos_des, self.z_pos_des)
            #print "Pitch angle: %.2f   Pitch torque: %.2f" % (self.pitch_angle_actual, self.dynamic_torque_pitch)
          
            # Scale gantry velocities
	    #if (self.gantry_cmd_vel.linear.x > 0.25):
            #    self.gantry_cmd_vel.linear.x = 0.25
	    #elif (self.gantry_cmd_vel.linear.x < -0.25):
            #    self.gantry_cmd_vel.linear.x = -0.25

            # Publish gantry velocity
            self.gantry_cmd_vel_pub.publish(self.gantry_cmd_vel)
            r.sleep()

if __name__ == '__main__':
    try:
        move_gantry = MoveGantryPID()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.is_running = False
        t.join()
    except rospy.ROSInterruptException: pass
