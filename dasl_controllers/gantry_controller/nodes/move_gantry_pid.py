#!/usr/bin/env python

PKG = 'gantry_controller'

import roslib; roslib.load_manifest(PKG)

import time
from threading import Thread
import math
#from pid import PID

import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import JointState
import tf

class MoveGantryPID():
    def __init__(self):

        self.is_running = True
        self.step_size = 0.05
	self.gantry_cmd_vel = Twist()
        self.joint_states = None
        self.joy_data = None
       
        #self.pid_x = PID (2.0, 0.0, 1.2)
        #self.pid_y = PID (2.0, 0.0, 1.2)
        self.K_P = 2.0
        self.K_I = 0.0
        self.K_D = 1.0
        self.Derivator_x = 0
        self.Derivator_y = 0

        rospy.init_node('move_gantry_pid', anonymous=True)
        rospy.Subscriber('/joy_gantry', Joy, self.read_joystick_data)
        rospy.Subscriber('/joint_states', JointState, self.joint_state_handler)
	self.gantry_cmd_vel_pub = rospy.Publisher('cmd_vel', Twist)

    def joint_state_handler(self, msg):
	self.joint_states = msg

    def read_joystick_data(self, data):
        self.joy_data = data

    def update_gantry_velocity(self):

        listener = tf.TransformListener()
        listener.waitForTransform("/mocap_origin", "/gantry_base", rospy.Time(), rospy.Duration(4.0))
        (trans,rot) = listener.lookupTransform('/mocap_origin', '/gantry_base', rospy.Time())

	self.x_pos_des = trans[0]
	self.y_pos_des = trans[1]
	self.z_pos_des = trans[2]

        while self.is_running:

            now = rospy.Time.now()
            listener.waitForTransform("/mocap_origin", "/gantry_base", now, rospy.Duration(4.0))
            (trans,rot) = listener.lookupTransform('/mocap_origin', '/gantry_base', now)

            if self.joy_data:
                self.x_pos_des += -1 * self.joy_data.axes[1] * self.step_size
                self.y_pos_des += -1 * self.joy_data.axes[0] * self.step_size
                self.z_pos_des += 1 * self.joy_data.axes[2] * self.step_size

            #self.pid_x.setPoint(self.x_pos_des)
            #self.pid_y.setPoint(self.y_pos_des)

            self.x_error = self.x_pos_des - trans[0]
            self.y_error = self.y_pos_des - trans[1]
            
            self.Px_value = self.K_P * self.x_error
            self.Dx_value = self.K_D * (self.x_error - self.Derivator_x)
            self.Derivator_x = self.x_error
            self.PID_x = self.Px_value + self.Dx_value

            self.Py_value = self.K_P * self.y_error
            self.Dy_value = self.K_D * (self.y_error - self.Derivator_y)
            self.Derivator_y = self.y_error
            self.PID_y = self.Py_value + self.Dy_value

            self.gantry_cmd_vel.linear.x = self.PID_x #self.K_P * self.x_error
            self.gantry_cmd_vel.linear.y = self.PID_y #self.K_P * self.y_error

            #print "Y_des: %.2f Y_act: %.2f PID_Y: %.2f" % (self.y_pos_des, trans[1], self.y_error)
            #print "X_des: %.2f X_act: %.2f PID_X: %.2f" % (self.x_pos_des, trans[0], self.x_error)

            if self.joint_states:
                torque_yaw = round(self.joint_states.effort[0],0)
                torque_pitch = round(self.joint_states.effort[1],0) + 6.6
                torque_roll = round(self.joint_states.effort[2],0) + 7.6

                if ((torque_pitch > 1) or (torque_pitch < -1)):
                    #self.x_pos_des -= torque_pitch * 0.01
                    self.gantry_cmd_vel.linear.x -= 1 * torque_pitch * 0.1

                if (torque_roll > 1) or (torque_roll < -1):
                    self.gantry_cmd_vel.linear.y += torque_roll * 0.1

                #if (torque_roll > 1):
                #    self.gantry_cmd_vel.linear.y += 0.3 #torque_roll * 0.1
                #elif (torque_roll < -1):
                    #self.y_pos_des += torque_roll * 0.01
                #    self.gantry_cmd_vel.linear.y -= 0.3 #torque_roll * 0.1

                #print "torque_roll: %.2f torque_pitch: %.2f" % (torque_roll, torque_pitch)

	    if (self.gantry_cmd_vel.linear.x > 0.25):
                self.gantry_cmd_vel.linear.x = 0.25
	    elif (self.gantry_cmd_vel.linear.x < -0.25):
                self.gantry_cmd_vel.linear.x = -0.25
	    if (self.gantry_cmd_vel.linear.y > 0.25):
                self.gantry_cmd_vel.linear.y = 0.25
	    elif (self.gantry_cmd_vel.linear.y < -0.25):
                self.gantry_cmd_vel.linear.y = -0.25

            self.gantry_cmd_vel_pub.publish(self.gantry_cmd_vel)
            time.sleep(0.1)

if __name__ == '__main__':
    try:
        move_gantry = MoveGantryPID()
        t = Thread(target=move_gantry.update_gantry_velocity)
        t.start()
        rospy.spin()
        move_gantry.is_running = False
        t.join()
    except rospy.ROSInterruptException: pass
