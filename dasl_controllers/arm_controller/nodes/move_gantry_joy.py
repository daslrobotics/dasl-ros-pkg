#!/usr/bin/env python

# Copyright (c) 2008, Willow Garage, Inc.  All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#     * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.  * Redistributions
#     in binary form must reproduce the above copyright notice, this list of
#     conditions and the following disclaimer in the documentation and/or
#     other materials provided with the distribution. # Neither the name of
#     the Willow Garage, Inc. nor the names of its contributors may be used to
#     endorse or promote products derived from this software without specific
#     prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 'AS IS'
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Author: Daniel Hewlett
# Author: Antons Rebguns

PKG = 'arm_controller'

import roslib; roslib.load_manifest(PKG)

import time
from math import pi
from threading import Thread

import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float64
from dynamixel_controllers.srv import *

class MoveGantryPS3():
    def __init__(self):
        self.is_running = True
        self.step_size = 5.0 * 3.14 / 180.0
        self.joy_data = None
        
        rospy.init_node('move_gantry_joy', anonymous=True)
        rospy.Subscriber('/joy', Joy, self.read_joystick_data)

        self.servo_position_x = rospy.Publisher('/x_controller/command', Float64)
        self.servo_position_y = rospy.Publisher('/y_controller/command', Float64)
        self.servo_position_z = rospy.Publisher('/z_controller/command', Float64)
        self.servo_position_yaw = rospy.Publisher('/yaw_controller/command', Float64)
        self.servo_position_pitch = rospy.Publisher('/pitch_controller/command', Float64)
        self.servo_position_roll = rospy.Publisher('/roll_controller/command', Float64)

	self.x_joint = 0.0
	self.y_joint = 0.0
	self.z_joint = 0.0
	self.yaw_joint = 0.0
	self.pitch_joint = 0.0
	self.roll_joint = 0.0

        rospy.wait_for_service('/x_controller/set_speed')
        self.servo_speed_x = rospy.ServiceProxy('/x_controller/set_speed', SetSpeed, persistent=True)
        rospy.wait_for_service('/y_controller/set_speed')
        self.servo_speed_y = rospy.ServiceProxy('/y_controller/set_speed', SetSpeed, persistent=True)
        rospy.wait_for_service('/z_controller/set_speed')
        self.servo_speed_z = rospy.ServiceProxy('/z_controller/set_speed', SetSpeed, persistent=True)
	
	self.servo_speed_x(1.0)
	self.servo_speed_y(1.0)
	self.servo_speed_z(1.0)

    def read_joystick_data(self, data):
        self.joy_data = data

    def update_gantry_velocity(self):
        while self.is_running:
            if self.joy_data:
		self.x_joint += 1 * self.joy_data.axes[0] * self.step_size
		self.y_joint += -1 * self.joy_data.axes[1] * self.step_size
		self.z_joint += -1 * self.joy_data.axes[3] * self.step_size
	    self.servo_position_x.publish(self.x_joint)
	    self.servo_position_y.publish(self.y_joint)
	    #self.servo_position_z.publish(self.z_joint)
	    self.servo_position_yaw.publish(self.yaw_joint)
	    self.servo_position_pitch.publish(self.pitch_joint)
	    self.servo_position_roll.publish(self.roll_joint)
            time.sleep(0.1)

if __name__ == '__main__':
    try:
        move_gantry = MoveGantryPS3()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.alive = False
        t.join()
    except rospy.ROSInterruptException: pass

