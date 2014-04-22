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

        self.gimbal_yaw_torque = 0
        self.gimbal_pitch_torque = 0
        self.gimbal_roll_torque = 0
        self.gimbal_yaw_pos = 0
        self.gimbal_pitch_pos = 0
        self.gimbal_roll_pos = 0
        self.right_shoulder_pitch_torque = 0
        self.right_elbow_pitch_torque = 0
        self.right_wrist_pitch_torque = 0
        self.right_shoulder_pitch_vel = 0
        self.right_elbow_pitch_vel = 0
        self.right_wrist_pitch_vel = 0

        #self.file.write("Time Yaw_torque Pitch_torque Roll_torque Shoulder_pitch_torque Elbow_pitch_torque Wrist_pitch_torque shoulder_vel elbow_vel wrist_vel \r\n\n")

        rospy.Subscriber('/joint_states', JointState, self.read_joint_state_data)

        r = rospy.Rate(30)

        while not rospy.is_shutdown():
            self.log_it()
            r.sleep()

    #def main(self):
            #self.sub_joint_state = rospy.Subscriber('/joint_states', JointState, self.read_joint_state_data)
            #rospy.spin()

    def read_joint_state_data(self, msg):
        self.gimbal_yaw_torque = msg.effort[0]
        self.gimbal_pitch_torque = msg.effort[1]
        self.gimbal_roll_torque = msg.effort[2]
        self.gimbal_yaw_pos = msg.position[0]
        self.gimbal_pitch_pos = msg.position[1]
        self.gimbal_roll_pos = msg.position[2]
        self.right_shoulder_pitch_torque = msg.effort[3]
        self.right_elbow_pitch_torque = msg.effort[6]
        self.right_wrist_pitch_torque = msg.effort[8]
        self.right_shoulder_pitch_vel = msg.velocity[3]
        self.right_elbow_pitch_vel = msg.velocity[6]
        self.right_wrist_pitch_vel = msg.velocity[8]
        #self.log_it()

    def log_it(self):
        #r = rospy.Rate(10) # 10hz
        self.file.write(str(time.time()-self.ts)+' '+
                        str(self.gimbal_yaw_torque)+' '+
                        str(self.gimbal_pitch_torque)+' '+
                        str(self.gimbal_roll_torque)+' '+
                        str(self.gimbal_yaw_pos)+' '+
                        str(self.gimbal_pitch_pos)+' '+
                        str(self.gimbal_roll_pos)+' '+
                        str(self.right_shoulder_pitch_torque)+' '+
                        str(self.right_elbow_pitch_torque)+' '+
                        str(self.right_wrist_pitch_torque)+' '+
                        str(self.right_shoulder_pitch_vel)+' '+
                        str(self.right_elbow_pitch_vel)+' '+
                        str(self.right_wrist_pitch_vel)+'\r\n')
	#if ((time.time()-self.ts) > 60):
	#    rospy.signal_shutdown("elapsed time")
        #r.sleep()
      
if __name__ == '__main__':
    try:
        log = Log()
        rospy.spin()
    except rospy.ROSInterruptException: pass
    #log.main()
