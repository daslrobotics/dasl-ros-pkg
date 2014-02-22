#!/usr/bin/env python

PKG = 'mk2_controller'

import roslib; roslib.load_manifest(PKG)

import time
import copy
import rospy
from sensor_msgs.msg import Joy
from sensor_msgs.msg import JointState as JointState
from std_srvs.srv import *

class MoveMK2Joy():
    def __init__(self):
        rospy.init_node('move_mk2_joy', anonymous=True)
        self.step_size = 1.0 * 0.005113269
        self.joy_data = None
        self.one_time = True

        self.position = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]

        self.joint_states = {'torso_yaw_joint',
                             'torso_pitch_joint',
                             'torso_roll_joint',
			     'joint_1',
                             'joint_2',
                             'joint_3',
                             'joint_4',
                             'joint_5',
                             'joint_6',
                             'joint_7',
			     'right_thumb_roll_joint',
                             'right_thumb_pitch_joint',
                             'right_index_yaw_joint',
                             'right_ring_yaw_joint'}

        rospy.Subscriber('/joy', Joy, self.read_joystick_data)
        self.joint_states_pub = rospy.Publisher('/command', JointState)
        rospy.Subscriber('/joint_states', JointState, self.read_joint_state_data)

        self.srv_home = rospy.ServiceProxy('/home', Empty)
        self.srv_stop = rospy.ServiceProxy('/stop', Empty)

        rospy.sleep(3)
        self.one_time = False 

        r = rospy.Rate(50)

        while not rospy.is_shutdown():
            self.publish_joint_states()
            r.sleep()

    def read_joystick_data(self, data):
        self.joy_data = data

    def read_joint_state_data(self, msg):

        self.joint_data = msg

        if self.one_time:
            self.position[0] = msg.position[0]
            self.position[1] = msg.position[1]
            self.position[2] = msg.position[2]
            self.position[3] = msg.position[3]
            self.position[4] = msg.position[4]
            self.position[5] = msg.position[5]
            self.position[6] = msg.position[6]
            self.position[7] = msg.position[7]
            self.position[8] = msg.position[8]
            self.position[9] = msg.position[9]
            self.position[10] = msg.position[10]
            self.position[11] = msg.position[11]
            self.position[12] = msg.position[12]
            self.position[13] = msg.position[13]

	    self.velocity = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]

    def publish_joint_states(self):
        # Construct message & publish joint states
        msg = JointState()
        if self.joy_data:

            if self.joy_data.buttons[10]:
                rospy.wait_for_service('home')
                resp = self.srv_home()
                self.joy_data = None
                msg.position = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]
                msg.velocity = [0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2]
                #msg.position.append(self.position)
                #msg.velocity.append(self.velocity)
                #msg.effort.append(0.0)
                msg.header.stamp = rospy.Time.now()
                self.joint_states_pub.publish(msg)

            elif self.joy_data.buttons[8]:
                rospy.wait_for_service('stop')
                resp = self.srv_stop()
                self.joy_data = None
                msg.velocity.append(0)

            elif self.joy_data.buttons[6]:

                des_pos = 1.57
                dev = 0.01
                msg.name.append('joint_4')
                msg.position.append(1.57)
                msg.velocity.append(0.2)
                msg.effort.append(0.0)
                msg.header.stamp = rospy.Time.now()
                self.joint_states_pub.publish(msg)
                while ((self.joint_data.position[6] < (des_pos - dev)) or 
                       (self.joint_data.position[6] > (des_pos + dev))):
                    print self.joint_data.position[6]

                alt = 1
                des_pos = 0.785

                while (1):

                    msg = JointState()
                    msg.name.append('joint_1')
                    msg.velocity.append(0.2)
                    msg.effort.append(0.0)
                    msg.position.append(alt * des_pos)
                    msg.header.stamp = rospy.Time.now()
                    self.joint_states_pub.publish(msg)
                
                    while ((self.joint_data.position[3] < (alt * des_pos - dev)) or 
                           (self.joint_data.position[3] > (alt * des_pos + dev))):
		        #rospy.sleep(1)
                        print self.joint_data.position[3]
		    alt *= -1

                #while self.joint_data.position
                #while (msg.

		# pitch forward: -0.785
		# pitch backward: 0.785

'''
            else:

                for joint in self.joint_states:
                    msg.name.append(joint)

                    if joint == 'torso_yaw_joint':
                        if self.joy_data.buttons[1]:
                            self.position[0] += -1 * self.joy_data.axes[2] * self.step_size
                        msg.position.append(self.position[0])
                        msg.velocity.append(0.2)

                    elif joint == 'torso_pitch_joint':
                        if self.joy_data.buttons[1]:
                            self.position[1] += -1 * self.joy_data.axes[1] * self.step_size
                        msg.position.append(self.position[1])
                        msg.velocity.append(0.2)

                    elif joint == 'torso_roll_joint':
                        if self.joy_data.buttons[1]:
                            self.position[2] += -1 * self.joy_data.axes[0] * self.step_size
                        msg.position.append(self.position[2])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_1':
                        if not self.joy_data.buttons[0]:
                            self.position[3] += -1 * self.joy_data.axes[1] * self.step_size
                        msg.position.append(self.position[3])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_2':
                        self.position[4] += -1 * self.joy_data.axes[0] * self.step_size
                        msg.position.append(self.position[4])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_3':
                        if not self.joy_data.buttons[0]:
                            self.position[5] += -1 * self.joy_data.axes[2] * self.step_size
                        msg.position.append(self.position[5])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_4':
                        if self.joy_data.buttons[0]:
                            self.position[6] += -1 * self.joy_data.axes[1] * self.step_size
                        #msg.position.append(self.position[6])
                        #msg.velocity.append(0.2)

                    elif joint == 'joint_5':
                        if self.joy_data.buttons[0]:
                            self.position[7] += -1 * self.joy_data.axes[2] * self.step_size
                        msg.position.append(self.position[7])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_6':
                        self.position[8] += -1 * self.joy_data.axes[5] * self.step_size
                        msg.position.append(self.position[8])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_7':
                        self.position[9] += -1 * self.joy_data.axes[4] * self.step_size
                        msg.position.append(self.position[9])
                        msg.velocity.append(0.2)

                    elif joint == 'right_thumb_roll_joint':
                        if self.joy_data.buttons[2]:
                            self.position[10] += -1 * self.step_size
                            self.velocity[10] = 0.5
                        elif self.joy_data.buttons[3]:
                            self.position[10] += 1 * self.step_size
                            self.velocity[10] = 0.5
                        elif self.joy_data.buttons[11]:
			    self.position[10] = -1.57
                            self.velocity[10] = 0.5
                        msg.position.append(self.position[10])
			msg.velocity.append(self.velocity[10])

                    elif joint == 'right_thumb_pitch_joint':
                        if self.joy_data.buttons[4]:
                            self.position[11] += 1 * self.step_size
                            self.velocity[11] = 0.5
                        elif self.joy_data.buttons[5]:
                            self.position[11] += -1 * self.step_size
                            self.velocity[11] = 0.5
                        elif self.joy_data.buttons[9]:
			    self.position[11] = 1.3
                            self.velocity[11] = 0.5
                        elif self.joy_data.buttons[7]:
			    self.position[11] = 0.0
                            self.velocity[11] = 0.75
                        msg.position.append(self.position[11])
			msg.velocity.append(self.velocity[11])

                    elif joint == 'right_index_yaw_joint':
                        if self.joy_data.buttons[4]:
                            self.position[12] += 1 * self.step_size
                            self.velocity[12] = 0.5
                        elif self.joy_data.buttons[5]:
                            self.position[12] += -1 * self.step_size
                            self.velocity[12] = 0.5
                        elif self.joy_data.buttons[9]:
			    self.position[12] = 1.31
                            self.velocity[12] = 0.5
                        elif self.joy_data.buttons[7]:
			    self.position[12] = 0.0
                            self.velocity[12] = 0.75
                        msg.position.append(self.position[12])
			msg.velocity.append(self.velocity[12])

                    elif joint == 'right_ring_yaw_joint':
                        if self.joy_data.buttons[4]:
                            self.position[13] += 1 * self.step_size
                            self.velocity[13] = 0.5
                        elif self.joy_data.buttons[5]:
                            self.position[13] += -1 * self.step_size
                            self.velocity[13] = 0.5
                        elif self.joy_data.buttons[9]:
			    self.position[13] = 1.42
                            self.velocity[13] = 0.5
                        elif self.joy_data.buttons[7]:
			    self.position[13] = 0.0
                            self.velocity[13] = 0.75
                        msg.position.append(self.position[13])
			msg.velocity.append(self.velocity[13])

                    else:
                        msg.position.append(0.0)

                    msg.effort.append(0.0)
                msg.header.stamp = rospy.Time.now()
                self.joint_states_pub.publish(msg)
'''

if __name__ == '__main__':
    try:
        s = MoveMK2Joy()
        rospy.spin()
    except rospy.ROSInterruptException: pass
