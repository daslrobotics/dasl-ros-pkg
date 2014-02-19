#!/usr/bin/env python

PKG = 'arm_controller'

import roslib; roslib.load_manifest(PKG)

import time
from math import pi
from threading import Thread

import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float64
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Vector3

class MoveGantryPS3():
    def __init__(self):
        self.is_running = True
        self.step_size = 0.1 #5.0 * 3.14 / 180.0
        self.joy_data = None

        self.gantry_publisher = rospy.Publisher('/gantry/velocity', Twist)

	self.gantry_pose = Twist()
	self.gantry_vel = Vector3()
	self.gantry_rpy = Vector3()

	self.gantry_vel.x = 0.0
	self.gantry_vel.y = 0.0
	self.gantry_vel.z = 0.0
	self.gantry_pose.linear = self.gantry_vel
	self.gantry_rpy.x = 0.0 # roll
	self.gantry_rpy.y = 0.0 # pitch
	self.gantry_rpy.z = 0.0 # yaw
	self.gantry_pose.angular = self.gantry_rpy

        rospy.init_node('gantry_joy', anonymous=True)
        rospy.Subscriber('/joy', Joy, self.read_joystick_data)

    def read_joystick_data(self, data):
        self.joy_data = data

    def update_gantry_velocity(self):
        while self.is_running:
            if self.joy_data:
			self.gantry_pose.linear.x = 1 * self.joy_data.axes[1] * self.step_size
			self.gantry_pose.linear.y = 1 * self.joy_data.axes[0] * self.step_size
			self.gantry_pose.linear.z = -1 * self.joy_data.axes[3] * self.step_size
			self.gantry_pose.angular.z = -1 * self.joy_data.axes[2] * self.step_size
			self.gantry_pose.angular.y = self.joy_data.axes[4]*0.15
			self.gantry_pose.angular.x = self.joy_data.axes[5]*0.15
	    print "---------------------------------"
	    print "Position",self.gantry_vel.x, self.gantry_vel.y, self.gantry_vel.z
	    print "RPY",self.gantry_rpy.x, self.gantry_rpy.y, self.gantry_rpy.z
	    print "---------------------------------"
 	    self.gantry_publisher.publish(self.gantry_pose)
            time.sleep(0.1)

if __name__ == '__main__':
    try:
        move_gantry = MoveGantryPS3()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.alive = False
        t.join()
    except rospy.ROSInterruptException: pass
