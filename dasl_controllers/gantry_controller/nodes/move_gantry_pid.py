#!/usr/bin/env python

PKG = 'gantry_controller'

import roslib; roslib.load_manifest(PKG)

import time
from threading import Thread
import math
from pid import PID

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import JointState
from nav_msgs.msg import Odometry
import tf

class MoveGantryPID():
    def __init__(self):

        self.is_running = True
        self.step_size = 0.05
	self.torque_gain = 0.5
	self.gantry_cmd_vel = Twist()
        self.joint_states = None
        self.joy_data = None
	self.odom_data = None

        # Set PID Gains
        self.pid_x = PID (2.0, 0, 0)
        self.pid_y = PID (2.0, 0, 0)
        self.pid_z = PID (2.0, 0, 0)

        rospy.init_node('move_gantry_pid', anonymous=True)
        rospy.Subscriber('joy', Joy, self.read_joystick_data)
        rospy.Subscriber('joint_states', JointState, self.joint_state_handler)
        rospy.Subscriber('odom', Odometry, self.read_odom_data)
        self.joint_states_pub = rospy.Publisher('/command', JointState)
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

        # Intialize desired position with first odom position
	self.x_pos_des = self.odom_data.pose.pose.position.x
	#self.y_pos_des = self.odom_data.pose.pose.position.y
	#self.z_pos_des = self.odom_data.pose.pose.position.z

        # Get static torque values
        #self.static_torque_roll = self.joint_states.effort[2]
        self.static_torque_pitch = self.joint_states.effort[1]
        #self.static_torque_yaw = self.joint_states.effort[0]

        r = rospy.Rate(10)

        while self.is_running:

            # Update PID with new setpoint
            #self.pid_x.setPoint(self.x_pos_des)
            #self.pid_y.setPoint(self.y_pos_des)
            #self.pid_z.setPoint(self.z_pos_des)

            # Update PID with new measurement 
            #self.gantry_cmd_vel.linear.x = self.pid_x.update(self.odom_data.pose.pose.position.x)
            #self.gantry_cmd_vel.linear.y = self.pid_y.update(self.odom_data.pose.pose.position.y)
            #self.gantry_cmd_vel.linear.z = self.pid_z.update(self.odom_data.pose.pose.position.z)

            # Get setpoint from joystick
            if self.joy_data:
                #self.x_pos_des += -1 * self.joy_data.axes[1] * self.step_size
                #self.y_pos_des += -1 * self.joy_data.axes[0] * self.step_size
                #self.z_pos_des += 1 * self.joy_data.axes[2] * self.step_size

                self.gantry_cmd_vel.linear.x = 1 * self.joy_data.axes[1] #* self.step_size




if self.joy_data.buttons[0]:




            #Command torso pitch		

            msg = JointState()
            msg.name = 'torso_pitch_joint'
            msg.position = 
            msg.velocity = 
	    msg.header.stamp = rospy.Time.now()
            self.joint_states_pub.publish(msg)









            print "X pos: %.5f  X vel: %.5f" % (self.odom_data.pose.pose.position.x, self.odom_data.twist.twist.linear.x)
	    #print "X des: %.5f  Y des: %.5f  Z des: %.5f" % (self.x_pos_des, self.y_pos_des, self.z_pos_des)

            # Introduce dynamic torque as a disturbance
	    if self.joint_states:

                # Calculate dynamic torques
		#self.dynamic_torque_roll = self.joint_states.effort[2] - self.static_torque_roll
		self.dynamic_torque_pitch = self.joint_states.effort[1] - self.static_torque_pitch 

		self.pitch_angle_actual = self.joint_states.position[1] 

		#self.pitch_angle_desired

                print "Pitch angle: %.2f   Pitch torque: %.2f" % (self.pitch_angle_actual, self.dynamic_torque_pitch)

                # Add as disturbance
                #self.gantry_cmd_vel.linear.x += 1 * self.dynamic_torque_pitch * self.torque_gain
                #self.gantry_cmd_vel.linear.y += 1 * self.dynamic_torque_roll * self.torque_gain
          
            # Scale gantry velocities
	    if (self.gantry_cmd_vel.linear.x > 0.25):
                self.gantry_cmd_vel.linear.x = 0.25
	    elif (self.gantry_cmd_vel.linear.x < -0.25):
                self.gantry_cmd_vel.linear.x = -0.25
	    if (self.gantry_cmd_vel.linear.y > 0.25):
                self.gantry_cmd_vel.linear.y = 0.25
	    if (self.gantry_cmd_vel.linear.z > 0.25):
                self.gantry_cmd_vel.linear.z = 0.25
	    elif (self.gantry_cmd_vel.linear.z < -0.25):
                self.gantry_cmd_vel.linear.z = 0.25

            # Publish gantry velocity
            self.gantry_cmd_vel_pub.publish(self.gantry_cmd_vel)
            r.sleep()
            #time.sleep(0.02)

if __name__ == '__main__':
    try:
        move_gantry = MoveGantryPID()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.is_running = False
        t.join()
    except rospy.ROSInterruptException: pass
