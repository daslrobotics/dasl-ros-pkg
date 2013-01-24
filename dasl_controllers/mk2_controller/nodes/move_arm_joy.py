#!/usr/bin/env python

PKG = 'mk2_controller'

import roslib; roslib.load_manifest(PKG)

import time

import rospy
from sensor_msgs.msg import Joy
from sensor_msgs.msg import JointState as JointState

class MoveArmPS3():
    def __init__(self):
        rospy.init_node('move_arm_joy', anonymous=True)
        self.step_size = 1
        self.joy_data = None

        #self.joint_states = {'gimbal_yaw_joint': JointState(name='gimbal_yaw_joint', position=0.0, velocity=0.0, effort=0.0),
        #                     'gimbal_pitch_joint': JointState(name='gimbal_pitch_joint', position=0.0, velocity=0.0, effort=0.0),
        #                     'gimbal_roll_joint': JointState(name='gimbal_pitch_joint', position=0.0, velocity=0.0, effort=0.0)}

        self.joint_states = {'gimbal_yaw_joint',
                             'gimbal_pitch_joint',
                             'gimbal_roll_joint'}

        rospy.Subscriber('/joy', Joy, self.read_joystick_data)
        self.joint_states_pub = rospy.Publisher('/command', JointState)

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
            for joint in self.joint_states:
	        msg.name.append(joint)
		if joint == 'gimbal_yaw_joint':
                    msg.position.append(-1 * self.joy_data.axes[0] * self.step_size)
                elif joint == 'gimbal_pitch_joint':
                    msg.position.append(-1 * self.joy_data.axes[1] * self.step_size)
                elif joint == 'gimbal_roll_joint':
                    msg.position.append(-1 * self.joy_data.axes[2] * self.step_size)
                else:
                    msg.position.append(0.0)
                msg.velocity.append(1.0)
                msg.effort.append(0.0)

	    msg.header.stamp = rospy.Time.now()
            self.joint_states_pub.publish(msg)

if __name__ == '__main__':
    try:
        s = MoveArmPS3()
        rospy.spin()
    except rospy.ROSInterruptException: pass
