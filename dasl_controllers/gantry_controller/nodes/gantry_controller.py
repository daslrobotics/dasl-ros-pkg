#!/usr/bin/env python

PKG = 'gantry_controller'

import roslib; roslib.load_manifest(PKG)
import rospy
import math
import tf
from geometry_msgs.msg import Twist

import time

import socket
UDP_IP = "129.25.32.56"
UDP_PORT = 5005

class GantryController():
    def __init__(self):
        self.is_running = True
	self.gantry_cmd_vel = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        rospy.init_node('gantry_controller', anonymous=True)
	rospy.Subscriber("/cmd_vel", Twist, self.update_gantry_velocity)
	self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def update_gantry_velocity(self, msg):
	self.gantry_cmd_vel[0] = msg.linear.x
	self.gantry_cmd_vel[1] = msg.linear.y
	self.gantry_cmd_vel[2] = msg.linear.z
	self.gantry_cmd_vel[3] = msg.angular.x
	self.gantry_cmd_vel[4] = msg.angular.y
	self.gantry_cmd_vel[5] = msg.angular.z
	self.sock.sendto(str(self.gantry_cmd_vel), (UDP_IP, UDP_PORT))
	time.sleep(0.1)

if __name__ == '__main__':
    try:
        move_gantry = GantryController()
        rospy.spin()
    except rospy.ROSInterruptException: pass
