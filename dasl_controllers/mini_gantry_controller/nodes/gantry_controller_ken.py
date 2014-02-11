#!/usr/bin/env python

# ros stuffs
PKG = 'arm_controller'
import roslib; roslib.load_manifest(PKG)

import time
from math import pi
from threading import Thread

import rospy
from std_msgs.msg import Float64
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Vector3
from dynamixel_msgs.msg import JointState

# dyanamixel stuffs

import os
import dynamixel
import random
import sys
import subprocess
import optparse
import yaml

# actual class

class GantryControl():
	def __init__(self):
		# Startup the gantry's dynamixels
		parser = optparse.OptionParser()
		parser.add_option("-c","--clean",
				  action="store_true", dest="clean", default=False,
				  help="Ignore the settings.yaml file if it exists and prompt for new settigns")
		(options,args)=parser.parse_args()
		# look for settings
		settingsFile = 'settings.yaml'
		if not options.clean and os.path.exists(settingsFile):
			with open(settingsFile,'r') as fh:
				settings = yaml.load(fh)
		else:
			settings = {}
			if os.name == "posix":
				portPrompt = "Which port corresponds to USB2Dynamixel? \n"
				try:
					possiblePorts = subprocess.check_output('ls /dev/ | grep -i usb',
										shell=True).split()
					possiblePorts = ['/dev/' + port for port in possiblePorts]
				except subprocesss.CalledProcessError:
					sys.exit("USB2Dynamixel not found.")
				counter = 1
				portCount = len(possiblePorts)
				for port in possiblePorts:
					portPrompt += "\t" + str(counter) + " - " + port + "\n"
					counter += 1
				portPrompt += "Enter Choice: "
				portChoice = None
				while not portChoice:
					portTest = raw_input(portPrompt)
					portTest = int(portTest) if 1<=int(portTest)<=portCount else None
					if portTest:
						portChoice = possiblePorts[portTest - 1]
			else:
				portPrompt = "Plese enter the port name for the USB2Dynamixel: "
				portChoice = raw_input(portPrompt)
			settings['port'] = portChoice
			# Baud rate
			baudRate = None
			while not baudRate:
				brTest = raw_input("Please enter baud rate [Default: 1000000 bps]:")
				if not brTest:
					baudRate = 1000000
				else:
					baudRate = int(brTest) if 9600 <= int(brTest) <= 1000000 else None
			settings['baudrate'] = baudRate
			settings['lowestServoID']=200
			settings['highestServoID']=233
		# setup dynamixel network
		self.serial = dynamixel.SerialStream(port=settings['port'],baudrate=settings['baudrate'],timeout=1)
		self.net = dynamixel.DynamixelNetwork(self.serial)
		print "Scanning for Dynamixels..."
		self.net.scan(settings['lowestServoID'],settings['highestServoID'])
		self.myActuators = []

		for dyn in self.net.get_dynamixels():
			print dyn.id
			self.myActuators.append(self.net[dyn.id])
		print "Scanned..."
		if options.clean or not os.path.exists(settingsFile):
			axes = ['x','y','z','r','p','yaw'] # defines the axis that we are using
			dynamixels_used = []
			for axis in axes:
				number = None
				while not number:
					numTest = raw_input("Please enter dynamixel number for the "+ axis)
					number = int(numTest) if (0<int(numTest)<=255 and (not int(numTest) in dynamixels_used)) else None
				dynamixels_used.append(number)
				settings[axis] = number

			with open(settingsFile, 'w') as fh:
				yaml.dump(settings,fh)
				print("Settings have been saved")
		self.position_dyns = [ settings['x'] , settings['y'] , settings['z'] ]
		self.orientation_dyns = [ settings['r'] , settings['p'] ,settings['yaw'] ]

		if not self.myActuators:
			print 'No Dynamixels Found!'
			sys.exit(0)
		else:
			print "...Done"

		for actuator in self.myActuators:
			actuator.synchronized = True
			actuator.torque_enable = True
			actuator.torque_limit = 1023
			actuator.max_torque = 1023
			if (actuator.id in self.position_dyns or actuator.id == self.orientation_dyns[2]):
				actuator.moving_speed = 0 # always set start velocity to 0 for the position dynamixels
				actuator.cw_angle_limit = 0 # both cw and ccw angle limits must be set to 0 for wheel mode
				actuator.ccw_angle_limit= 0 # this double checks this fact
			elif actuator.id in self.orientation_dyns: # this is for roll and pitch
				#actuator.torque_enabled = False # comment out when you want it to be active
				actuator.moving_speed = 128
				actuator.cw_angle_limit = 0
				actuator.cw_angle_limit  = 4096 # this may need to be double checked if it's 4096 or 4095
			time.sleep(0.1)
	

		# Startup the subscriber and publisher services
		self.is_running = True
		self.velocity_data = None
		rospy.init_node('gantry_controller', anonymous=True)
		rospy.Subscriber('/gantry/velocity',Twist,self.update_velocities)

	def radToDynPos(self,rad):
		return int(4096*rad)+2048

	def radToDynVel(self,rad):
		if(rad>0):
			return int(rad * 1023)
		else:
			return 1024+int(-1 * rad * 1023)
	
	def dynToRadPos(self,dyn):
		return (dyn-2048)/4096

	def dynToRadVel(self,dyn):
		if(dyn > 1024):
			return -1*(dyn-1024)/1023
		else:
			return dyn/1023

	def update_velocities(self,data):
		self.velocity_data = data

	def move_gantry(self):
		# main control loop to run gantry and update rostopics
		while self.is_running:
			if self.velocity_data:
				for actuator in self.myActuators:
					if actuator.id == self.position_dyns[0]: # x	
						actuator.moving_speed = self.radToDynVel(self.velocity_data.linear.x)
					elif actuator.id == self.position_dyns[1]: # y
						actuator.moving_speed = self.radToDynVel(self.velocity_data.linear.y)
					elif actuator.id == self.position_dyns[2]: # z
						actuator.moving_speed = self.radToDynVel(-1*self.velocity_data.linear.z)
					elif actuator.id == self.orientation_dyns[2]: # yaw
						actuator.moving_speed = self.radToDynVel(0.5*self.velocity_data.angular.z)
					elif actuator.id == self.orientation_dyns[0]: # roll based on y
						actuator.goal_position = self.radToDynPos(-0.1* self.velocity_data.linear.y)
					elif actuator.id == self.orientation_dyns[1]: # pitch based on x
						actuator.goal_position = self.radToDynPos(0.1*self.velocity_data.linear.x)
					time.sleep(0.01)
				self.net.synchronize()
				print_statement = "---------------------"
				print_statement = print_statement +"\n" + "---------------------"
				print_statement = print_statement + "\n" + "---------------------"
				for actuator in self.myActuators:
					actuator.read_all()
					print_statement = print_statement+"\n"+str( actuator.id) + "-s-"+str( actuator.current_speed) + "-p-" + str( actuator.current_position)
				print print_statement
# startup stuffs

if __name__ == '__main__':
	try:
		gantry = GantryControl()
		t = Thread(target=gantry.move_gantry)
		t.start()
		rospy.spin()
		gantry.alive = False
		t.join()
	except rospy.ROSInterruptException: pass
