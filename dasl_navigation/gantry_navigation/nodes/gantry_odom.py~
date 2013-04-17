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

import time

import socket
UDP_IP = "129.25.16.43"
UDP_PORT = 5005

class GantryOdom():
    def __init__(self):
        self.is_running = True
	#self.gantry_cmd_vel = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        rospy.init_node('gantry_odom', anonymous=True)
	#rospy.Subscriber("/cmd_vel", Twist, self.update_gantry_velocity)
	#self.gantry_cmd_vel_pub = rospy.Publisher('cmd_vel', Twist)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
        self.sock.bind((UDP_IP, UDP_PORT))

    def update_gantry_odom(self):

        #data, addr = self.sock.recvfrom(1024) # buffer size is 1024 bytes
        print "received message:"#, data

	#self.gantry_cmd_vel[0] = msg.linear.x
	#self.gantry_cmd_vel[1] = msg.linear.y
	#self.gantry_cmd_vel[2] = msg.linear.z
	#self.gantry_cmd_vel[3] = msg.angular.x
	#self.gantry_cmd_vel[4] = msg.angular.y
	#self.gantry_cmd_vel[5] = msg.angular.z
	#self.sock.sendto(str(self.gantry_cmd_vel), (UDP_IP, UDP_PORT))
	#time.sleep(0.1)
        print "here"

if __name__ == '__main__':
    try:
        gantry_odom = GantryOdom()
        rate = rospy.Rate(10.0)
        while not rospy.is_shutdown():
            gantry_odom.update_gantry_odom
            rate.sleep()
        #t = Thread(target=gantry_odom.update_gantry_odom)
        #t.start()
        #rospy.spin()
        #gantry_odom.is_running = False
        #t.join()
    except rospy.ROSInterruptException: pass
