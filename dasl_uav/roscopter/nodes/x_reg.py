#!/usr/bin/env python

import roslib
import rospy
import termios
import sys
import gtk
import struct
import os
import time
import pygtk
import tty
import select
roslib.load_manifest('roscopter')
import roscopter.msg
import threading

pygtk.require('2.0')

from std_msgs.msg import String, Header, Int32
from geometry_msgs.msg import Twist, Pose
from std_msgs.msg import Empty
from sensor_msgs.msg import Joy, Imu, NavSatStatus, NavSatFix
from datetime import datetime

class PID:

	def __init__(self):

		self.pub_log = rospy.Publisher('log_x', String)
		self.pub_reg = rospy.Publisher('reg_x', Int32)
		rospy.init_node('x_regulator')

		self.Kp = 600
		self.Ki = 10
		self.Kd = 650
		self.Integrator = 0.0
		self.Integrator_max = 500
		self.Integrator_min = -500
		self.Derivator = 0.0
		
		self.x = 1500
		self.current_value = 0.0
		self.set_point = 0.0
		self.error = 0.0
		self.trigger = True
		self.press = 0
		self.make_log = String()
		self.mode = 'o'

		self.update()

	def reg_main(self):
		self.sub_pos = rospy.Subscriber("/Optitrack/RB0", Pose, self.pos_call)
		self.sub_joy = rospy.Subscriber("/quad_joy", Joy, self.joy_call)
		self.sub_mod = rospy.Subscriber('mode', String, self.set_mode)
		self.sub_int = rospy.Subscriber('send_rc', roscopter.msg.RC, self.integrator_set)
		rospy.spin()

	def set_mode(self, data):
		self.mode = data.data

	def pos_call(self,data):
		self.current_value = data.position.x

	def joy_call(self,data):
		if self.mode=='a' and data.buttons[9]==1:
			self.trigger = False
			self.Derivator = self.current_value
			self.press = 0
		elif self.mode=='o' or self.mode=='s' or self.mode=='a':
			self.trigger = True
			self.Integrator = 0.0
		elif self.mode=='p': #and self.press > 20 and data.buttons[9]==1:
			if data.buttons[2]==1:
				self.Kp = self.Kp + 0.001
				print "x : " + str(self.Kp)
				#self.set_point -= 0.2
				self.press = 0
			elif data.buttons[3]==1:
				self.Kp = self.Kp - 0.001
				print "x : " + str(self.Kp)
				#self.set_point += 0.2
				self.press = 0
			

	def integrator_set(self,data):
		if self.trigger:
			self.x = data.channel[1]

	def update(self):

		threading.Timer(0.0495,self.update).start()

		if self.press < 21:
			self.press += 1

		if self.trigger:
			self.set_point = self.current_value

		self.error = -(self.set_point - self.current_value)
		self.P_value = self.Kp * self.error
		self.D_value = self.Kd * (self.current_value - self.Derivator)
		self.Derivator = self.current_value

		self.Integrator = self.Integrator + self.error

		if self.Integrator > self.Integrator_max:
			self.Integrator = self.Integrator_max
		elif self.Integrator < self.Integrator_min:
			self.Integrator = self.Integrator_min

		self.I_value = self.Integrator * self.Ki

		if self.D_value > 50:
			self.D_value = 50
		elif self.D_value < -50:
			self.D_value = -50

		PID_reg = self.P_value + self.I_value + self.D_value + self.x

		if PID_reg > 1650:
			PID_reg = 1650
		elif PID_reg < 1350:
			PID_reg = 1350

		self.pub_reg.publish(Int32(int(PID_reg)))

		self.make_log.data = str(int(PID_reg))+' '+str(self.P_value)+' '+str(self.I_value)+' '+str(self.D_value)+' '+str(self.error)
		self.pub_log.publish(self.make_log)


if __name__ == '__main__':

	print 'process started at ' + str(datetime.now())
	var = PID()
	var.reg_main()

