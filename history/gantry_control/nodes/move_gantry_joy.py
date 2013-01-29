#!/usr/bin/env python

PKG = 'gantry_control'

import roslib; roslib.load_manifest(PKG)

import time
from threading import Thread

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist

class MoveGantryPS3():
    def __init__(self):
        self.is_running = True
        self.step_size = 20
	self.gantry_cmd_vel = Twist()
        self.joy_data = None
       
        rospy.init_node('move_gantry_joy', anonymous=True)
        rospy.Subscriber('/joy', Joy, self.read_joystick_data)
	self.gantry_cmd_vel_pub = rospy.Publisher('cmd_vel', Twist)

    def read_joystick_data(self, data):
        self.joy_data = data

    def update_gantry_velocity(self):
        while self.is_running:
            if self.joy_data:
                self.gantry_cmd_vel.linear.x = -1 * self.joy_data.axes[0] * self.step_size
                self.gantry_cmd_vel.linear.y = -1 * self.joy_data.axes[1] * self.step_size
                self.gantry_cmd_vel.linear.z = 1 * self.joy_data.axes[3] * self.step_size
                self.gantry_cmd_vel.angular.x = 1 * self.joy_data.axes[2] * self.step_size
                self.gantry_cmd_vel.angular.y = 1 * self.joy_data.axes[4] * self.step_size
                self.gantry_cmd_vel.angular.z = 1 * self.joy_data.axes[5] * self.step_size
	    self.gantry_cmd_vel_pub.publish(self.gantry_cmd_vel)
            time.sleep(0.1)

if __name__ == '__main__':
    try:
        move_gantry = MoveGantryPS3()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.is_running = False
        t.join()
    except rospy.ROSInterruptException: pass
