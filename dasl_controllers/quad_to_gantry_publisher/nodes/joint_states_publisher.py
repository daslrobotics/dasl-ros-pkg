#!/usr/bin/env python

import roslib; roslib.load_manifest('gantry_controller')
import rospy

from sensor_msgs.msg import JointState

import socket
UDP_IP = "129.25.35.173"
UDP_PORT = 5005

import re

class JointStateMessage():
    def __init__(self, name, position, velocity, effort):
        self.name = name
        self.position = position
        self.velocity = velocity
        self.effort = effort

class JointStatesPublisher():
    def __init__(self):

        rospy.init_node('gantry_joint_states_publisher', anonymous=True)
        
        self.joint_states = {'gantry_x_axis_joint': JointStateMessage('gantry_x_axis_joint', 0.0, 0.0, 0.0),
                             'gantry_y_axis_joint': JointStateMessage('gantry_y_axis_joint', 0.0, 0.0, 0.0),
                             'gantry_z_axis_joint': JointStateMessage('gantry_z_axis_joint', 0.0, 0.0, 0.0)}
                             
        self.controllers = ('gantry_x_axis_controller',
                            'gantry_y_axis_controller',
                            'gantry_z_axis_controller')
                           
	sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
	sock.bind((UDP_IP, UDP_PORT))

        # Start controller state subscribers
        #[rospy.Subscriber(c + '/state', JointState, self.controller_state_handler) for c in self.controllers]

        # Start publisher
        self.joint_states_pub = rospy.Publisher('joint_states', JointState)
        
        publish_rate = rospy.get_param('~rate', 50)
        r = rospy.Rate(publish_rate)
        
        while not rospy.is_shutdown():
	    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
	    gantry_pos = data.split(',')
    	    gantry_pos = map(float, gantry_pos) 
	    self.controller_state_handler(gantry_pos)
            self.publish_joint_states()
            r.sleep()
            
    def controller_state_handler(self, position):
        jcs = JointStateMessage("gantry_x_axis_joint", position[0], 0, 0)
        self.joint_states["gantry_x_axis_joint"] = jcs

        jcs = JointStateMessage("gantry_y_axis_joint", position[1], 0, 0)
        self.joint_states["gantry_y_axis_joint"] = jcs

        jcs = JointStateMessage("gantry_z_axis_joint", position[2], 0, 0)
        self.joint_states["gantry_z_axis_joint"] = jcs
        
    def publish_joint_states(self):
        # Construct message & publish joint states
        msg = JointState()
        msg.name = []
        msg.position = []
        msg.velocity = []
        msg.effort = []
        
        for joint in self.joint_states.values():
            msg.name.append(joint.name)
            msg.position.append(joint.position)
            msg.velocity.append(joint.velocity)
            msg.effort.append(joint.effort)
            
        msg.header.stamp = rospy.Time.now()
        self.joint_states_pub.publish(msg)

if __name__ == '__main__':
    try:
        s = JointStatesPublisher()
        rospy.spin()
    except rospy.ROSInterruptException: pass

