#!/usr/bin/env python

import roslib
import rospy

from sensor_msgs.msg import JointState

import socket
import re

#UDP_IP = "129.25.32.56"
UDP_IP = "129.25.35.173"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    gantry_pos = data.split(',')
    gantry_pos = map(float, gantry_pos) 
    print "received message:", gantry_pos
