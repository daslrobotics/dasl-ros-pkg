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
roslib.load_manifest('roscopter')
import roscopter.msg

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
        self.pub_rc = rospy.Publisher('send_rc', roscopter.msg.RC)
        self.pub_mode = rospy.Publisher('mode',String)

        rospy.init_node('main_control_node')
        self.pub_mode.publish('o')
        self.axes = []
        self.buttons = []
        self.control = 0
        self.twist = [0, 0, 0, 0, 1500, 1500, 1500, 1500]
        self.reg_x = 1500
        self.reg_y = 1500
        self.reg_z = 1100
        self.x = 1500
        self.y = 1500
        self.z = 1100
        self.yaw = 1500

    def inf_main(self):
        self.sub_joy = rospy.Subscriber("/quad_joy", Joy, self.joy_callback)
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

        self.x = 1500-self.axes[1]*250
        self.y = 1500-self.axes[0]*250
        self.z = 1100+((self.axes[3]+1)/2)*800 #800 #600
	self.yaw = 1500-self.axes[2]*300

        self.fly()

    def fly(self):

        if self.buttons[10]==1:
            self.control=0
            self.z=1000
            self.pub_mode.publish('o')
            print 'Stop quad'

        if self.buttons[8]==1 and self.control==0:
            self.yaw=2000
            self.z=1000
            print 'Arm quad'

        if self.buttons[6]==1:
            if (self.control==2) and (self.z <= self.reg_z+50) and (self.z >= self.reg_z-50):
                self.control=1
                self.pub_mode.publish('s')
                print 'Stabilize'
            elif (self.control==0) and (self.z == 1100):
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

        # RC Channels: [0]-pitch [1]-roll [2]-throttle [3]-yaw

        if self.control==0:
            (self.twist[0], self.twist[1], self.twist[2], self.twist[3]) = (1500, 1500, 1100, int(self.yaw))
        elif self.control==1:
            (self.twist[0], self.twist[1], self.twist[2], self.twist[3]) = (int(self.y), int(self.x), int(self.z), int(self.yaw))
        elif self.control==2:
            #(self.twist[0], self.twist[1], self.twist[2], self.twist[3]) = (int(self.y), int.reg_x, int(self.z), int(self.yaw))
            (self.twist[0], self.twist[1], self.twist[2], self.twist[3]) = (int(self.y), int(self.x), self.reg_z, int(self.yaw))
        elif self.control==3:
            (self.twist[0], self.twist[1], self.twist[2], self.twist[3]) = (self.reg_y, self.reg_x, self.reg_z, self.reg_yaw)

        self.pub_rc.publish(self.twist)
        print
        print self.reg_z, int(self.z)
        print

if __name__ == '__main__':

    print 'process started at ' + str(datetime.now())
    var = MCN()
    var.inf_main()
