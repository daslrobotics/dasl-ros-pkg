#!/usr/bin/env python

import roslib
import rospy
import termios
import sys
import gtk
import struct
import os
import time
import pygtk
import tty
import select
roslib.load_manifest('arm_controller')

from std_srvs.srv import *

pygtk.require('2.0')

from std_msgs.msg import String, Header, Int32
from geometry_msgs.msg import Twist, Pose
from std_msgs.msg import Empty
from sensor_msgs.msg import Joy, Imu, NavSatStatus, NavSatFix
from datetime import datetime
from std_msgs.msg import Float64

class MCN():

    def __init__(self):
        self.right_shoulder_roll_pub = rospy.Publisher('right_shoulder_roll_controller/command', Float64)
        self.right_wrist_roll_pub = rospy.Publisher('right_wrist_roll_controller/command', Float64)
        self.left_shoulder_roll_pub = rospy.Publisher('left_shoulder_roll_controller/command', Float64)
        self.left_wrist_roll_pub = rospy.Publisher('left_wrist_roll_controller/command', Float64)

        rospy.init_node('main_control_node')
        self.axes = []
        self.buttons = []

	self.right_shoulder_roll_joint = 0.0
	self.right_wrist_roll_joint = 0.0
	self.left_shoulder_roll_joint = 0.0
	self.left_wrist_roll_joint = 0.0
        self.step_size = 0.5 * 3.14 / 180.0

    def inf_main(self):
        self.sub_joy = rospy.Subscriber("/joy", Joy, self.joy_callback)
        rospy.spin()

    def joy_callback(self, data):
        self.axes = data.axes
        self.buttons = data.buttons
        self.fly()

    def fly(self):

        if self.buttons[4]==1:
	    self.right_shoulder_roll_joint = 0.0
	    self.right_wrist_roll_joint = 0.0
	    self.left_shoulder_roll_joint = 0.0
	    self.left_wrist_roll_joint = 0.0

        if self.buttons[5]==1:
	    self.right_shoulder_roll_joint = 0.0
	    self.right_wrist_roll_joint = 0.0
	    self.left_shoulder_roll_joint = 0.0
	    self.left_wrist_roll_joint = 0.0

        #self.arm_shoulder_pitch_joint += -1 * self.axes[0] * self.step_size
        #self.arm_elbow_pitch_joint += -1 * self.axes[1] * self.step_size
        #self.arm_wrist_pitch_joint += -1 * self.axes[2] * self.step_size
        #self.arm_gripper_joint += -1 * self.axes[3] * self.step_size

	self.right_shoulder_roll_pub.publish(self.right_shoulder_roll_joint)
	self.right_wrist_roll_pub.publish(self.right_wrist_roll_joint)
	#self.left_shoulder_roll_pub.publish(self.left_shoulder_roll_joint)
	#self.left_wrist_roll_pub.publish(self.left_wrist_roll_joint)

if __name__ == '__main__':

    print 'process started at ' + str(datetime.now())
    var = MCN()
    var.inf_main()
