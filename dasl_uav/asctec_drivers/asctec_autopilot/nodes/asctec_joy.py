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
roslib.load_manifest('asctec_autopilot')
#import roscopter.msg

from asctec_msgs.msg import *

import numpy as np

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

        self.thrust_pub = rospy.Publisher('/asctec/CTRL_INPUT', CtrlInput)

        self.pub_mode = rospy.Publisher('mode',String)
        self.arm_shoulder_pitch_pub = rospy.Publisher('shoulder_pitch_controller/command', Float64)
        self.arm_elbow_pitch_pub = rospy.Publisher('elbow_pitch_controller/command', Float64)
        self.arm_wrist_pitch_pub = rospy.Publisher('wrist_pitch_controller/command', Float64)
        self.arm_gripper_pub = rospy.Publisher('gripper_controller/command', Float64)

        rospy.init_node('main_control_node')
        self.pub_mode.publish('o')
        self.axes = []
        self.buttons = []
        self.control = 0
        self.reg_x = 0
        self.reg_y = 0
        self.reg_z = 0
        self.x = 0
        self.y = 0
        self.z = 0
        self.yaw = 0
	self.gripper = 1500

	self.arm_shoulder_pitch_joint = -1.57
	self.arm_elbow_pitch_joint = -1.20
	self.arm_wrist_pitch_joint = 0.0
	self.arm_gripper_joint = 0.0
        self.step_size = 0.5 * 3.14 / 180.0

    def inf_main(self):
        self.sub_joy = rospy.Subscriber("/joy", Joy, self.joy_callback)
        self.sub_regx = rospy.Subscriber('reg_x', Int32, self.get_regx)
        self.sub_regy = rospy.Subscriber('reg_y', Int32, self.get_regy)
        self.sub_regz = rospy.Subscriber('reg_z', Int32, self.get_regz)
        self.sub_regyaw = rospy.Subscriber('reg_yaw', Int32, self.get_regyaw)
        rospy.spin()

    def get_regx(self, data):
        self.reg_x = data.data
        self.fly()

    def get_regy(self, data):
        self.reg_y = data.data
        self.fly()

    def get_regz(self, data):
        self.reg_z = data.data
        self.fly()

    def get_regyaw(self, data):
        self.reg_yaw = data.data
        self.fly()

    def joy_callback(self, data):
        self.axes = data.axes
        self.buttons = data.buttons

        self.x = 0-self.axes[0]*1023
        self.y = 0-self.axes[1]*1023
        self.z = 0+((self.axes[3]+1)/2)*4095 #4095
	self.yaw = 0-self.axes[2]*300

        self.fly()

    def fly(self):

        msg = CtrlInput()

        if self.buttons[10]==1:
            self.control=0
            self.z=0
            self.pub_mode.publish('o')
            print 'Stop quad'

        if self.buttons[8]==1 and self.control==0:
            self.control=1
            self.x = 0
            self.y = 0
            self.z= 0
            self.yaw=-2048
            print 'Arm quad'

        if self.buttons[6]==1:
            self.control=1
            self.pub_mode.publish('s')
            print 'Stabilize'

        if self.buttons[11]==1 and (self.control==1 or self.control==3):
            self.control=2
            self.pub_mode.publish('a')
            print 'AltHold'

        if self.buttons[9]==1 and self.control==2: 
            self.control=3
            self.pub_mode.publish('p')
            print 'PosHold'

        if self.buttons[4]==1:
	    self.arm_shoulder_pitch_joint = 0.0
	    self.arm_elbow_pitch_joint = 0.0
	    self.arm_wrist_pitch_joint = 0.0
	    self.arm_gripper_joint = 0.0

        if self.buttons[5]==1:
	    self.arm_shoulder_pitch_joint = -1.57
	    self.arm_elbow_pitch_joint = -1.20
	    self.arm_wrist_pitch_joint = 0.0
	    self.arm_gripper_joint = 0.0

        #self.arm_shoulder_pitch_joint += -1 * self.axes[0] * self.step_size
        #self.arm_elbow_pitch_joint += -1 * self.axes[1] * self.step_size
        #self.arm_wrist_pitch_joint += -1 * self.axes[2] * self.step_size
        #self.arm_gripper_joint += -1 * self.axes[3] * self.step_size

	self.arm_shoulder_pitch_pub.publish(self.arm_shoulder_pitch_joint)
	self.arm_elbow_pitch_pub.publish(self.arm_elbow_pitch_joint)
	self.arm_wrist_pitch_pub.publish(self.arm_wrist_pitch_joint)
	self.arm_gripper_pub.publish(self.arm_gripper_joint)

        if self.control==0:
            (msg.roll, msg.pitch, msg.thrust, msg.yaw) = (0, 0, 0, 0)
        elif self.control==1:
            (msg.roll, msg.pitch, msg.thrust, msg.yaw) = (int(self.x), int(self.y), int(self.z), int(self.yaw))
        elif self.control==2:
            (msg.roll, msg.pitch, msg.thrust, msg.yaw) = (int(self.x), int(self.y), self.reg_z, self.reg_yaw)
        elif self.control==3:
            (msg.roll, msg.pitch, msg.thrust, msg.yaw) = (self.reg_y, self.reg_x, self.reg_z, self.reg_yaw)

        msg.ctrl = 15 #0xF
        msg.chksum = np.int16(msg.pitch + msg.roll + msg.yaw + msg.thrust + msg.ctrl + 43690)  # + int('0xAAAA',16)

	#self.twist[5] = self.gripper

        self.thrust_pub.publish(msg)
        print
        print self.reg_z, int(self.z), msg.chksum
        print

if __name__ == '__main__':

    print 'process started at ' + str(datetime.now())
    var = MCN()
    var.inf_main()
