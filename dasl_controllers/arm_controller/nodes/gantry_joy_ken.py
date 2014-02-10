#!/usr/bin/env python

PKG = 'arm_controller'

import roslib; roslib.load_manifest(PKG)

import time
from math import pi
from threading import Thread

import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float64
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Point
from geometry_msgs.msg import Quaternion

class MoveGantryPS3():
    def __init__(self):
        self.is_running = True
        self.step_size = 5.0 * 3.14 / 180.0
        self.joy_data = None

        self.gantry_publisher = rospy.Publisher('/gantry', Pose)

	self.gantry_pose = Pose()
	self.gantry_point = Point()
	self.gantry_rpy = Quaternion()

	self.gantry_point.x = 0.0
	self.gantry_point.y = 0.0
	self.gantry_point.z = 0.0
	self.gantry_pose.position = self.gantry_point
	self.gantry_rpy.x = 0.0 # roll
	self.gantry_rpy.y = 0.0 # pitch
	self.gantry_rpy.z = 0.0 # yaw
	self.gantry_rpy.w = 0.0 # ignored
	self.gantry_pose.orientation = self.gantry_rpy

        rospy.init_node('gantry_joy', anonymous=True)
        rospy.Subscriber('/joy', Joy, self.read_joystick_data)

    def read_joystick_data(self, data):
        self.joy_data = data

    def update_gantry_velocity(self):
        while self.is_running:
            if self.joy_data:
		self.gantry_pose.position.x += 1 * self.joy_data.axes[1] * self.step_size
		self.gantry_pose.position.y += 1 * self.joy_data.axes[0] * self.step_size
		self.gantry_pose.position.z += -1 * self.joy_data.axes[3] * self.step_size
		self.gantry_pose.orientation.z += -1 * self.joy_data.axes[2] * self.step_size
		self.gantry_pose.orientation.y = self.joy_data.axes[4]*0.15
		self.gantry_pose.orientation.x = self.joy_data.axes[5]*0.15

	    print "Point",self.gantry_point.x, self.gantry_point.y, self.gantry_point.z
	    print "RPY",self.gantry_rpy.x, self.gantry_rpy.y, self.gantry_rpy.z
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

