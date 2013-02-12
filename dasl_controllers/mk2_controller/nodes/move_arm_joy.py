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
        rospy.init_node('move_arm_joy', anonymous=True)
        self.step_size = 1.0 * 0.005113269
        self.joy_data = None

        self.position = [0,0,0,0,0,0,0,0,0,0,0] 

        self.joint_states = {'right_shoulder_pitch_joint',
                             'right_shoulder_roll_joint',
                             'right_shoulder_yaw_joint',
                             'right_elbow_pitch_joint',
                             'right_wrist_yaw_joint',
                             'right_wrist_pitch_joint',
                             'right_wrist_roll_joint',
                             'right_thumb_yaw_joint',
                             'right_thumb_pitch_joint',
                             'right_index_finger_joint',
                             'right_middle_finger_joint'}

        rospy.Subscriber('/joy', Joy, self.read_joystick_data)#'/joy_mk2_arm',
        #rospy.Subscriber('/joy_arm', Joy, self.read_joystick_data)#'/joy_mk2_arm',
        self.joint_states_pub = rospy.Publisher('/command', JointState)

        self.srv_home = rospy.ServiceProxy('/home', Empty)
        self.srv_stop = rospy.ServiceProxy('/stop', Empty)

        r = rospy.Rate(50)

        while not rospy.is_shutdown():
            self.publish_joint_states()
            r.sleep()

    def read_joystick_data(self, data):
        self.joy_data = data

    def publish_joint_states(self):
        # Construct message & publish joint states
        msg = JointState()
        if self.joy_data:

            if self.joy_data.buttons[10]:
                rospy.wait_for_service('home')
                resp = self.srv_home()
                self.joy_data = None
                self.position = [0,0,0,0,0,0,0,0,0,0,0]

            elif self.joy_data.buttons[1]:
                rospy.wait_for_service('stop')
                resp = self.srv_stop()
                self.joy_data = None
                msg.velocity.append(0)

            else:

                for joint in self.joint_states:
                    msg.name.append(joint)

                    if joint == 'right_shoulder_pitch_joint':
                        if not self.joy_data.buttons[0]:
                            self.position[0] += -1 * self.joy_data.axes[1] * self.step_size
                        msg.position.append(self.position[0])
                        msg.velocity.append(0.2)

                    elif joint == 'right_shoulder_roll_joint':
                        self.position[1] += -1 * self.joy_data.axes[0] * self.step_size
                        msg.position.append(self.position[1])
                        msg.velocity.append(0.2)

                    elif joint == 'right_shoulder_yaw_joint':
                        if not self.joy_data.buttons[0]:
                            self.position[2] += -1 * self.joy_data.axes[2] * self.step_size
                        msg.position.append(self.position[2])
                        msg.velocity.append(0.2)

                    elif joint == 'right_elbow_pitch_joint':
                        if self.joy_data.buttons[0]:
                            self.position[3] += -1 * self.joy_data.axes[1] * self.step_size
                        msg.position.append(self.position[3])
                        msg.velocity.append(0.2)

                    elif joint == 'right_wrist_yaw_joint':
                        if self.joy_data.buttons[0]:
                            self.position[4] += -1 * self.joy_data.axes[2] * self.step_size
                        msg.position.append(self.position[4])
                        msg.velocity.append(0.2)

                    elif joint == 'right_wrist_pitch_joint':
                        self.position[5] += 1 * self.joy_data.axes[5] * self.step_size
                        msg.position.append(self.position[5])
                        msg.velocity.append(0.2)

                    elif joint == 'right_wrist_roll_joint':
                        self.position[6] += -1 * self.joy_data.axes[4] * self.step_size
                        msg.position.append(self.position[6])
                        msg.velocity.append(0.2)

                    elif joint == 'right_thumb_yaw_joint':
                        if self.joy_data.buttons[2]:
                            self.position[7] += -1 * self.step_size
                        elif self.joy_data.buttons[3]:
                            self.position[7] += 1 * self.step_size
                        msg.position.append(self.position[7])
                        msg.velocity.append(0.2)

                    elif joint == 'right_thumb_pitch_joint':
                        if self.joy_data.buttons[4]:
                            self.position[8] += 1 * self.step_size
                        elif self.joy_data.buttons[5]:
                            self.position[8] += -1 * self.step_size
                        msg.position.append(self.position[8])
                        msg.velocity.append(0.2)

                    elif joint == 'right_index_finger_joint':
                        if self.joy_data.buttons[4]:
                            self.position[9] += -1 * self.step_size
                        elif self.joy_data.buttons[5]:
                            self.position[9] += 1 * self.step_size
                        msg.position.append(self.position[9])
                        msg.velocity.append(0.2)

                    elif joint == 'right_middle_finger_joint':
                        if self.joy_data.buttons[4]:
                            self.position[10] += -1 * self.step_size
                        elif self.joy_data.buttons[5]:
                            self.position[10] += 1 * self.step_size
                        msg.position.append(self.position[10])
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
