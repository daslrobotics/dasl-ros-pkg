#!/usr/bin/env python

# Copyright (c) 2010, Arizona Robotics Research Group, University of Arizona
# All rights reserved.
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

import roslib; roslib.load_manifest('gantry_controllers')
import rospy

from sensor_msgs.msg import JointState

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
                            
        # Start controller state subscribers
        [rospy.Subscriber(c + '/state', JointState, self.controller_state_handler) for c in self.controllers]
        
        # Start publisher
        self.joint_states_pub = rospy.Publisher('joint_states', JointState)
        
        publish_rate = rospy.get_param('~rate', 50)
        r = rospy.Rate(publish_rate)
        
        while not rospy.is_shutdown():
            self.publish_joint_states()
            r.sleep()
            
    def controller_state_handler(self, msg):
        jcs = JointStateMessage(msg.name, msg.current_pos, msg.velocity, msg.load)
        self.joint_states[msg.name] = jcs
        
     def publish_joint_states(self):
        # Construct message & publish joint states
        msg = JointState
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

