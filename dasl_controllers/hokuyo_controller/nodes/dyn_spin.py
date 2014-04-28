#!/usr/bin/env python

# MUST REDO THIS FILE--THIS IS NOT THE PROPER WAY I WAS JUST BEING A LAZY FUCK
import os
import time

while True:
	os.system("rostopic pub -1 /hokuyo_spin_controller/command std_msgs/Float64 -- 1.57")
	time.sleep(5)
	os.system("rostopic pub -1 /hokuyo_spin_controller/command std_msgs/Float64 -- -1.57")
	time.sleep(5)
