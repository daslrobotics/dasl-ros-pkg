#!/usr/bin/env python  
import roslib
roslib.load_manifest('hokuyo_controller')
import rospy
import os

from dynamixel_msgs.msg import JointState
import tf


def handle_dyn_pose(msg):
    br = tf.TransformBroadcaster()
    br.sendTransform((0, 0, 0),
                     tf.transformations.quaternion_from_euler(0, 0, -msg.current_pos),
                     rospy.Time.now(),
                     "laser_yaw",
                     "laser_base")

if __name__ == '__main__':
    rospy.init_node('hokuyo_tf_broadcaster')
    rospy.Subscriber('/hokuyo_spin_controller/state',
                     JointState,
                     handle_dyn_pose)
    rospy.spin()
