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

class MCN():

    def __init__(self):
        rospy.init_node('main_control_node')
        self.axes = []
        self.buttons = []
        self.control = 0
        self.twist = [0, 0, 0, 0, 1500, 1500, 1500, 1500]
        self.x = 1500.0
        self.y = 1500.0
        self.z = 1000.0
        self.yaw = 1500
        self.pub_rc = rospy.Publisher('/send_rc', roscopter.msg.RC)
        self.sub_joy = rospy.Subscriber("/joy", Joy, self.joy_callback)

        r = rospy.Rate(20)
        while not rospy.is_shutdown():
            self.fly()
            r.sleep()

    def joy_callback(self, data):
        self.axes = data.axes
        self.buttons = data.buttons
        self.x = 1500-self.axes[0]*300
        self.y = 1500-self.axes[1]*300
        self.z = 1124+((self.axes[3]+1)/2)*600
	self.yaw = 1500-self.axes[2]*300

    def fly(self):

        if self.buttons:

            if self.buttons[10]:
                self.control = 0
                print 'Off'

            if self.buttons[8] and self.control == 0:
                self.yaw=1917
                self.z=1124
                print 'Arm quad'

            if self.buttons[6]:
                if (self.control == 0):
                    self.control = 1
                    print 'Stabilize quad'

        if self.control==0:
            (self.twist[0], self.twist[1], self.twist[2], self.twist[3]) = (1499, 1499, 1124, int(self.yaw))
        elif self.control==1:
            (self.twist[0], self.twist[1], self.twist[2], self.twist[3]) = (int(self.x), int(self.y), int(self.z), int(self.yaw))

        self.pub_rc.publish(self.twist)

if __name__ == '__main__':
    print 'process started at ' + str(datetime.now())
    try:
        var = MCN()
        rospy.spin()
    except rospy.ROSInterruptException: pass
