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
from dynamixel_msgs.msg import JointState as JointState

class MoveArmPS3():
    def __init__(self):
        self.is_running = True
        self.step_size = 1.0 * 3.14 / 180.0
	#2.0 * 0.005113269	
        self.joy_data = None
	self.gripper_open = False
        self.prev_time = time.time()
        
        rospy.init_node('move_arm_joy', anonymous=True)
        rospy.Subscriber('/arm_joy', Joy, self.read_joystick_data)

        rospy.Subscriber('/shoulder_pitch_controller/state', JointState, self.read_joint_state_data)

        self.servo_position_shoulder_pitch = rospy.Publisher('/shoulder_pitch_controller/command', Float64)
        self.servo_position_elbow_pitch = rospy.Publisher('/elbow_pitch_controller/command', Float64)
        self.servo_position_wrist_pitch = rospy.Publisher('/wrist_pitch_controller/command', Float64)
        self.servo_position_gripper = rospy.Publisher('/gripper_controller/command', Float64)

        self.shoulder_pitch_joint = 2.17
	self.elbow_pitch_joint = -2.18
	self.wrist_pitch_joint = -1.58
	self.gripper_joint = 0

    def read_joystick_data(self, data):
        self.joy_data = data
        cur_time = time.time()
        timediff = cur_time - self.prev_time
        self.prev_time = cur_time
	if data.buttons[1] and timediff > 0.1: self.toggle_gripper()

    def read_joint_state_data(self, msg):
        self.joint_data = msg

    def toggle_gripper(self):
        if self.gripper_open: self.gripper_joint = 0.00
        else: self.gripper_joint = -1.5
        self.gripper_open = not self.gripper_open

    def update_arm_position(self):
        while self.is_running:
            if self.joy_data:
		# Stow
		if self.joy_data.buttons[10] == 1:
		    self.shoulder_pitch_joint = 2.17
		    self.elbow_pitch_joint = -2.18
		    self.wrist_pitch_joint = -1.58
		    self.gripper_joint = 0
		# Grab posture
		if self.joy_data.buttons[8] == 1:
		    self.shoulder_pitch_joint = 0.690
		    self.elbow_pitch_joint = -1.51
		    self.wrist_pitch_joint = -0.798
		    self.gripper_joint = 0
		# Straight down
		if self.joy_data.buttons[6] == 1:
		    self.shoulder_pitch_joint = 0
		    self.elbow_pitch_joint = 0
		    self.wrist_pitch_joint = 0
		    self.gripper_joint = 0

		if self.joy_data.buttons[11] == 1:

                    alt = 1
                    des_pos = 0.785
                    dev = 0.05

		    while not rospy.is_shutdown():

		        self.shoulder_pitch_joint = alt * des_pos

	    	        self.servo_position_shoulder_pitch.publish(self.shoulder_pitch_joint)
                
                        while ((self.joint_data.current_pos < (alt * des_pos - dev)) or 
                               (self.joint_data.current_pos > (alt * des_pos + dev))):
                            print self.joint_data.current_pos
		        alt *= -1

                if not self.joy_data.buttons[0]:
		    self.shoulder_pitch_joint += -1 * self.joy_data.axes[1] * self.step_size
                if self.joy_data.buttons[0]:
		    self.elbow_pitch_joint += -1 * self.joy_data.axes[1] * self.step_size
		self.wrist_pitch_joint += -1 * self.joy_data.axes[3] * self.step_size
	        self.gripper_joint += -1 * self.joy_data.axes[4] * self.step_size

	    self.servo_position_shoulder_pitch.publish(self.shoulder_pitch_joint)
	    self.servo_position_elbow_pitch.publish(self.elbow_pitch_joint)
	    self.servo_position_wrist_pitch.publish(self.wrist_pitch_joint)
	    self.servo_position_gripper.publish(self.gripper_joint)

            time.sleep(0.05)

if __name__ == '__main__':
    try:
        move_arm = MoveArmPS3()
        t = Thread(target=move_arm.update_arm_position)
        t.start()
        rospy.spin()
        move_arm.alive = False
        t.join()
    except rospy.ROSInterruptException: pass

