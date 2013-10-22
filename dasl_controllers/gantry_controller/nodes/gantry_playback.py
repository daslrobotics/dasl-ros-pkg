#!/usr/bin/env python

PKG = 'gantry_controller'

import roslib; roslib.load_manifest(PKG)

import time
from threading import Thread
import math
from pid import PID

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist, Pose

class MoveGantryPlayback():

    def __init__(self):

        self.is_running = True
        self.step_size = 0.05
	self.gantry_cmd_vel = Twist()

	self.current_x = 0.0
	self.current_y = 0.0
	self.current_z = 0.0

        # Set PID Gains
        self.pid_x = PID (0.1, 0.0, 0.0)
        self.pid_y = PID (0.0, 0.0, 0.0)
        self.pid_z = PID (0.0, 0.0, 0.0)

        rospy.init_node('move_gantry_playback', anonymous=True)
	self.sub_pos = rospy.Subscriber("/Optitrack/RB0", Pose, self.pos_call)
	self.gantry_cmd_vel_pub = rospy.Publisher('/cmd_vel', Twist)

    def pos_call(self,data):
	self.current_x = data.position.x
	self.current_y = data.position.y
	self.current_z = data.position.z

    def update_gantry_velocity(self):

        # Intialize desired position with first odom position
	self.x_pos_des = 147
	self.y_pos_des = 558
	self.z_pos_des = 2063

        r = rospy.Rate(10)

	while self.is_running:

            # Update PID with new setpoint
            self.pid_x.setPoint(self.x_pos_des)
            self.pid_y.setPoint(self.y_pos_des)
            self.pid_z.setPoint(self.z_pos_des)

            # Update PID with new measurement 
            self.gantry_cmd_vel.linear.x = self.pid_x.update(self.current_x)
            self.gantry_cmd_vel.linear.y = self.pid_y.update(self.current_y)
            self.gantry_cmd_vel.linear.z = self.pid_z.update(self.current_z)
          
            # Scale gantry velocities
	    if (self.gantry_cmd_vel.linear.x > 0.25):
                self.gantry_cmd_vel.linear.x = 0.25
	    elif (self.gantry_cmd_vel.linear.x < -0.25):
                self.gantry_cmd_vel.linear.x = -0.25
	    if (self.gantry_cmd_vel.linear.y > 0.25):
                self.gantry_cmd_vel.linear.y = 0.25
	    if (self.gantry_cmd_vel.linear.z > 0.25):
                self.gantry_cmd_vel.linear.z = 0.25
	    elif (self.gantry_cmd_vel.linear.z < -0.25):
                self.gantry_cmd_vel.linear.z = 0.25

            # Publish gantry velocity
            #self.gantry_cmd_vel_pub.publish(self.gantry_cmd_vel)
            r.sleep()

if __name__ == '__main__':
    try:
        move_gantry = MoveGantryPlayback()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.is_running = False
        t.join()
    except rospy.ROSInterruptException: pass
