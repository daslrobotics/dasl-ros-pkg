#!/usr/bin/env python

import socket

#UDP_IP = "10.0.0.5"
UDP_PORT = 5005
UDP_IP = "129.25.32.56"
#UDP_PORT = 10000
x_vel = '1'
y_vel = '2'
z_vel = '3'

#MESSAGE = "x"+x_vel+"y"+y_vel+"z"+z_vel
MESSAGE = "[1,2,3]"

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
