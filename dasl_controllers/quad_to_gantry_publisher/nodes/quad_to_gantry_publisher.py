#!/usr/bin/env python

PKG = 'gantry_controller'

import roslib; roslib.load_manifest(PKG)

import time
from threading import Thread

import rospy
#from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry

import socket
UDP_IP = "129.25.32.56"
UDP_PORT = 5005

class GantryController():
    def __init__(self):
        self.is_running = True
	self.gantry_cmd_vel = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
	self.odom_data = None
        rospy.init_node('gantry_controller', anonymous=True)
	rospy.Subscriber("/ground_truth/state", Odometry, self.get_odom_data)
	self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def get_odom_data(self, data):
        self.odom_data = data

    def update_gantry_velocity(self):
        while self.is_running:
	    if self.odom_data:
	        self.gantry_cmd_vel[0] = self.odom_data.twist.twist.linear.x
	        self.gantry_cmd_vel[1] = self.odom_data.twist.twist.linear.y
	        self.gantry_cmd_vel[2] = self.odom_data.twist.twist.linear.z
	        self.gantry_cmd_vel[3] = self.odom_data.twist.twist.angular.x
	        self.gantry_cmd_vel[4] = self.odom_data.twist.twist.angular.y
	        self.gantry_cmd_vel[5] = self.odom_data.twist.twist.angular.z
	    self.sock.sendto(str(self.gantry_cmd_vel), (UDP_IP, UDP_PORT))
	    time.sleep(0.1)

if __name__ == '__main__':
    try:
        move_gantry = GantryController()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.is_running = False
        t.join()
    except rospy.ROSInterruptException: pass
