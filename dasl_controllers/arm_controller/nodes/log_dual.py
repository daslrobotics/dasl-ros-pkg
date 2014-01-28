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

pygtk.require('2.0')

from dynamixel_msgs.msg import JointState
from std_msgs.msg import String, Header, Int32, Float32, Float64
from sensor_msgs.msg import Joy
from datetime import datetime

class Log():

    def __init__(self):

        self.ts = time.time()
        file_id = str(datetime.now())
        self.file = open('Logs/LOG-'+file_id+'.txt','w')

        rospy.init_node('log')

        self.gimbal_yaw_pos = 0.0
	self.gimbal_yaw_tor = 0.0
        self.rsr = 0.0
	self.rwr = 0.0
        self.lsr = 0.0
	self.lwr = 0.0
        self.option = 0
        self.file.write("Time            Gimbal_Position    Gimbal_Yaw    Right_Shoulder_Roll Right_Wrist_Roll Left_Shoulder_Roll   Left_Wrist_Roll\r\n\n")

    def main(self):
        self.sub_gimbal_yaw = rospy.Subscriber("/yaw_controller/state", JointState, self.get_gimbal_yaw)
        self.sub_rsr = rospy.Subscriber("/right_shoulder_roll_controller/state", JointState, self.get_rsr)
        self.sub_rwr = rospy.Subscriber("/right_wrist_roll_controller/state", JointState, self.get_rwr)
        self.sub_lsr = rospy.Subscriber("/left_shoulder_roll_controller/state", JointState, self.get_lsr)
        self.sub_lwr = rospy.Subscriber("/left_wrist_roll_controller/state", JointState, self.get_lwr)
        rospy.spin()

    def get_gimbal_yaw(self, data):
        self.gimbal_yaw_pos = data.current_pos
	self.gimbal_yaw_tor = data.load

    def get_rsr(self, data):
        self.rsr = data.load

    def get_rwr(self, data):
        self.rwr = data.load

    def get_lsr(self, data):
        self.lsr = data.load

    def get_lwr(self, data):
        self.lwr = data.load
        self.log_it()

    def log_it(self):
        self.file.write(str(time.time()-self.ts)+' '+str(self.gimbal_yaw_pos)+' '+str(self.gimbal_yaw_tor)+' '+str(self.rsr)+' '+str(self.rwr)+' '+str(self.lsr)+' '+str(self.lwr)+'\r\n')
      
if __name__ == '__main__':

    log = Log()
    log.main()
