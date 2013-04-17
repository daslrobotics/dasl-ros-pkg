#!/usr/bin/env python
import roslib
roslib.load_manifest('dasl_mocap')
import rospy
import math
import tf

if __name__ == '__main__':
    rospy.init_node('mocap_frame_listener')

    listener = tf.TransformListener()

    listener.waitForTransform("/mocap_origin", "/gantry_base", rospy.Time(), rospy.Duration(4.0))

    rate = rospy.Rate(10.0)

    while not rospy.is_shutdown():
        now = rospy.Time.now()
        listener.waitForTransform("/mocap_origin", "/gantry_base", now, rospy.Duration(4.0))
        (trans,rot) = listener.lookupTransform('/mocap_origin', '/gantry_base', now)
        print (trans[0])
        rate.sleep()
