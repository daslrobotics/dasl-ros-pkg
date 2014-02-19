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

        self.gimbal_yaw = 0.0
        self.gimbal_pitch = 0.0
        self.gimbal_roll = 0.0

        self.option = 0
        self.file.write("Time            Yaw Pitch Roll \r\n\n")

    def main(self):
        self.sub_gimbal_yaw = rospy.Subscriber("/yaw_controller/state", JointState, self.get_gimbal_yaw)
        self.sub_gimbal_pitch = rospy.Subscriber("/pitch_controller/state", JointState, self.get_gimbal_pitch)
        self.sub_gimbal_roll = rospy.Subscriber("/roll_controller/state", JointState, self.get_gimbal_roll)
        rospy.spin()

    def get_gimbal_yaw(self, data):
        self.gimbal_yaw = data.load

    def get_gimbal_pitch(self, data):
        self.gimbal_pitch = data.load

    def get_gimbal_roll(self, data):
        self.gimbal_roll = data.load
        self.log_it()

    def log_it(self):
        self.file.write(str(time.time()-self.ts)+'   '+str(self.gimbal_yaw)+'      '+str(self.gimbal_pitch)+'      '+str(self.gimbal_roll)+'\r\n')
      
if __name__ == '__main__':

    log = Log()
    log.main()
