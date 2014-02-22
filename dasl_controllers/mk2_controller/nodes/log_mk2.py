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
roslib.load_manifest('mk2_controller')

pygtk.require('2.0')

from sensor_msgs.msg import JointState as JointState
from std_msgs.msg import String, Header, Int32, Float32, Float64
from sensor_msgs.msg import Joy
from datetime import datetime

class Log():

    def __init__(self):

        self.ts = time.time()
        file_id = str(datetime.now())
        self.file = open('Logs/LOG-'+file_id+'.txt','w')

        rospy.init_node('log')

        self.file.write("Time Yaw_torque Pitch_torque Roll_torque Shoulder_Pitch_torque Shoulder_Pitch_position\r\n\n")

    def main(self):
            self.sub_joint_state = rospy.Subscriber('/joint_states', JointState, self.read_joint_state_data)
            rospy.spin()

    def read_joint_state_data(self, msg):
        self.gimbal_yaw_torque = msg.effort[0]
        self.gimbal_pitch_torque = msg.effort[1]
        self.gimbal_roll_torque = msg.effort[2]
        self.right_shoulder_pitch_torque = msg.effort[3]
        self.right_shoulder_pitch_position = msg.position[3]
        self.log_it()

    def log_it(self):
        r = rospy.Rate(30) # 10hz
        self.file.write(str(time.time()-self.ts)+' '+
                        str(self.gimbal_yaw_torque)+' '+
                        str(self.gimbal_pitch_torque)+' '+
                        str(self.gimbal_roll_torque)+' '+
                        str(self.right_shoulder_pitch_torque)+' '+
                        str(self.right_shoulder_pitch_position)+'\r\n')
        r.sleep()
      
if __name__ == '__main__':
    log = Log()
    log.main()
