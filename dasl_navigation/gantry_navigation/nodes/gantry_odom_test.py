#!/usr/bin/env python

PKG = 'gantry_navigation'

import roslib; roslib.load_manifest(PKG)
import rospy
import math
import tf
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Pose
from nav_msgs.msg import Odometry
from threading import Thread

import socket

UDP_IP = "129.25.16.43"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print "received message:", data
