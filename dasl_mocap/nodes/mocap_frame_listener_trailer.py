#!/usr/bin/env python  
import roslib
roslib.load_manifest('dasl_mocap')

import rospy
import tf

if __name__ == '__main__':
    rospy.init_node('gantry_base_trailer_tf_broadcaster')
    br = tf.TransformBroadcaster()
    rate = rospy.Rate(10.0)
    while not rospy.is_shutdown():
        br.sendTransform((0.0, 1.0, 0.0),
                         (0.0, 0.0, 0.0, 1.0),
                         rospy.Time.now(),
                         "gantry_base_trailer",
                         "mocap_origin")
        rate.sleep()
