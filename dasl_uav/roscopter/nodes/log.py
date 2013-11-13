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

pygtk.require('2.0')

from std_msgs.msg import String, Header, Int32, Float32
from geometry_msgs.msg import Twist, Pose
from std_msgs.msg import Empty
from sensor_msgs.msg import Joy, Imu, NavSatStatus, NavSatFix
from datetime import datetime

class Log():

    def __init__(self):

        self.ts = time.time()
        file_id = str(datetime.now())
        self.file = open('Logs/LOG-'+file_id+'.txt','w')
        self.file_z = open('Logs/z-LOG-'+file_id+'.txt','w')
        self.file_y = open('Logs/y-LOG-'+file_id+'.txt','w')
        self.file_x = open('Logs/x-LOG-'+file_id+'.txt','w')

        rospy.init_node('logovi')

        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
        self.control = [0, 0, 0, 0]
        self.PIDx = ''
        self.PIDy = ''
        self.PIDz = ''

        self.position = ''
        self.controls = ''
        self.option = 0

        self.file_x.write("Time           X-Position    Pitch PID   P   I    D          Error\r\n\n")
        self.file_y.write("Time           Y-Position    Roll  PID   P   I    D          Error\r\n\n")

    def main(self):
        self.sub_position = rospy.Subscriber("/Optitrack/RB0", Pose, self.get_position)
        self.sub_control = rospy.Subscriber('send_rc', roscopter.msg.RC, self.get_control)
        self.sub_regx = rospy.Subscriber('log_x', String, self.get_PIDx)
        self.sub_regy = rospy.Subscriber('log_y', String, self.get_PIDy)
        self.sub_regz = rospy.Subscriber('log_z', String, self.get_PIDz)
        rospy.spin()

    def get_PIDx(self, data):
        self.PIDx = data.data

    def get_PIDy(self, data):
        self.PIDy = data.data

    def get_PIDz(self, data):
        self.PIDz = data.data
        self.log_it()

    def get_control(self, data):
        self.control[0] = data.channel[0]
        self.control[1] = data.channel[1]
        self.control[2] = data.channel[2]
        self.control[3] = data.channel[3]
        self.controls = str(self.control[0])+' '+str(self.control[1])+' '+str(self.control[2])+' '+str(self.control[3])

    def get_position(self, data):
        self.x = data.position.x
        self.y = data.position.y
        self.z = data.position.z
        self.position = str(self.x)+' '+str(self.y)+' '+str(self.z)

    def log_it(self):
        self.file_x.write(str(time.time()-self.ts)+' '+str(self.x)+' '+str(self.control[1])+' '+self.PIDx+'\r\n')
        self.file_y.write(str(time.time()-self.ts)+' '+str(self.y)+' '+str(self.control[0])+' '+self.PIDy+'\r\n')
        self.file_z.write(str(time.time()-self.ts)+' '+str(self.z)+' '+str(self.control[2])+' '+self.PIDz+'\r\n')
        self.file.write(str(time.time()-self.ts)+' '+self.position+' '+self.controls+'\r\n')
        
        if self.option == '1' or self.option == 'x':
            print self.PIDx
        elif self.option == '2' or self.option == 'y':
            print self.PIDy
        elif self.option == '3' or self.option == 'z':
            print self.PIDz
        elif self.option == '4':
            print self.position, self.controls
        else:
            print 'logging...'

if __name__ == '__main__':

    print 'Format: 1.time 2.position_data 3.control_data 4.PID'
    print '...................................................'
    print 'choose print mode:'
    print '1. PIDx'
    print '2. PIDy'
    print '3. PIDz'
    print '4. Position and control data'
    pick = raw_input('->')
    log = Log()
    log.option = pick
    log.main()
