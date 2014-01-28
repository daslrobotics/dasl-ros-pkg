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

        self.gimbal_pitch = 0.0
        self.elbow_pitch = 0.0
        self.wrist_pitch = 0.0

        self.option = 0
        self.file.write("Time            Gimbal Pitch     Elbow Pitch     Wrist Pitch\r\n\n")

    def main(self):
        self.sub_gimbal_pitch = rospy.Subscriber("/pitch_controller/state", JointState, self.get_gimbal_pitch)
        self.sub_elbow_pitch = rospy.Subscriber("/elbow_pitch_controller/state", JointState, self.get_elbow_pitch)
        self.sub_wrist_pitch = rospy.Subscriber("/wrist_pitch_controller/state", JointState, self.get_wrist_pitch)
        rospy.spin()

    def get_gimbal_pitch(self, data):
        self.gimbal_pitch = data.load

    def get_elbow_pitch(self, data):
        self.elbow_pitch = data.load

    def get_wrist_pitch(self, data):
        self.wrist_pitch = data.load
        self.log_it()

    def log_it(self):
        self.file.write(str(time.time()-self.ts)+'   '+str(self.gimbal_pitch)+'      '+str(self.elbow_pitch)+'      '+str(self.wrist_pitch)+'\r\n')
      
if __name__ == '__main__':

    log = Log()
    log.main()
