#!/usr/bin/env python

PKG = 'mk2_controller'

import roslib; roslib.load_manifest(PKG)

import time
from threading import Thread

import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float64

class MoveArmPS3():
    def __init__(self):
        self.is_running = True
        self.step_size = 1
	self.arm_cmd_pos = 0
        self.joy_data = None
       
        rospy.init_node('move_arm_joy', anonymous=True)
        rospy.Subscriber('/joy', Joy, self.read_joystick_data)
	self.arm_pub = rospy.Publisher('/yaw_joint_controller/command', Float64)

    def read_joystick_data(self, data):
        self.joy_data = data

    def update_arm_position(self):
        while self.is_running:
            if self.joy_data:
                self.arm_cmd_pos = -1 * self.joy_data.axes[0] * self.step_size
	    self.arm_pub.publish(self.arm_cmd_pos)
            time.sleep(0.1)

if __name__ == '__main__':
    try:
        move_arm = MoveArmPS3()
        t = Thread(target=move_arm.update_arm_position)
        t.start()
        rospy.spin()
        move_arm.is_running = False
        t.join()
    except rospy.ROSInterruptException: pass
