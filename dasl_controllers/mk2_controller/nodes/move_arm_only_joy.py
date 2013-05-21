#!/usr/bin/env python

PKG = 'mk2_controller'

import roslib; roslib.load_manifest(PKG)

import time
import copy
import rospy
from sensor_msgs.msg import Joy
from sensor_msgs.msg import JointState as JointState
from std_srvs.srv import *

class MoveArmPS3():
    def __init__(self):
        rospy.init_node('move_arm_only_joy', anonymous=True)
        self.step_size = 1.0 * 0.005113269
        self.joy_data = None
        self.one_time = True

        self.position = [0,0,0,0,0,0,0]

        self.joint_states = {'joint_1',
                             'joint_2',
                             'joint_3',
                             'joint_4',
                             'joint_5',
                             'joint_6',
                             'joint_7'}

        rospy.Subscriber('/joy', Joy, self.read_joystick_data)#'/joy_mk2_arm',
        #rospy.Subscriber('/joy_arm', Joy, self.read_joystick_data)#'/joy_mk2_arm',
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
        if self.one_time:
            self.position[0] = msg.position[0]
            self.position[1] = msg.position[1]
            self.position[2] = msg.position[2]
            self.position[3] = msg.position[3]
            self.position[4] = msg.position[4]
            self.position[5] = msg.position[5]
            self.position[6] = msg.position[6]

    def publish_joint_states(self):
        # Construct message & publish joint states
        msg = JointState()
        if self.joy_data:

            if self.joy_data.buttons[10]:
                rospy.wait_for_service('home')
                resp = self.srv_home()
                self.joy_data = None
                self.position = [0,0,0,0,0,0,0]

            elif self.joy_data.buttons[1]:
                rospy.wait_for_service('stop')
                resp = self.srv_stop()
                self.joy_data = None
                msg.velocity.append(0)

            else:

                for joint in self.joint_states:
                    msg.name.append(joint)

                    if joint == 'joint_1':
                        if not self.joy_data.buttons[0]:
                            self.position[0] += -1 * self.joy_data.axes[1] * self.step_size
                        msg.position.append(self.position[0])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_2':
                        self.position[1] += -1 * self.joy_data.axes[0] * self.step_size
                        msg.position.append(self.position[1])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_3':
                        if not self.joy_data.buttons[0]:
                            self.position[2] += -1 * self.joy_data.axes[2] * self.step_size
                        msg.position.append(self.position[2])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_4':
                        if self.joy_data.buttons[0]:
                            self.position[3] += -1 * self.joy_data.axes[1] * self.step_size
                        msg.position.append(self.position[3])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_5':
                        if self.joy_data.buttons[0]:
                            self.position[4] += -1 * self.joy_data.axes[2] * self.step_size
                        msg.position.append(self.position[4])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_6':
                        self.position[5] += -1 * self.joy_data.axes[5] * self.step_size
                        msg.position.append(self.position[5])
                        msg.velocity.append(0.2)

                    elif joint == 'joint_7':
                        self.position[6] += -1 * self.joy_data.axes[4] * self.step_size
                        msg.position.append(self.position[6])
                        msg.velocity.append(0.2)

                    else:
                        msg.position.append(0.0)

                    msg.effort.append(0.0)
                msg.header.stamp = rospy.Time.now()
                self.joint_states_pub.publish(msg)

if __name__ == '__main__':
    try:
        s = MoveArmPS3()
        rospy.spin()
    except rospy.ROSInterruptException: pass
