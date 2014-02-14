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

from sensor_msgs.msg import JointState
from std_msgs.msg import String, Header, Int32, Float32, Float64
from sensor_msgs.msg import Joy
from datetime import datetime

class Log():

    def __init__(self):

        self.ts = time.time()
        file_id = str(datetime.now())
        self.file = open('Logs/LOG-'+file_id+'.txt','w')

        rospy.init_node('log')

        self.gantry_state = JointState()

        self.option = 0
        self.file.write("Time with joint info in some order\r\n\n")

    def main(self):
        self.sub_gantry_state = rospy.Subscriber("/gantry/joint_states", JointState, self.get_joint_state)
        rospy.spin()

    def get_joint_state(self, data):
        self.gantry_state = data.load

    def log_it(self):
        self.file.write('-------------------\n'+str(time.time()-self.ts)+'\n'+ str(gantry_state)+'\r\n')
      
if __name__ == '__main__':

    log = Log()
    log.main()
