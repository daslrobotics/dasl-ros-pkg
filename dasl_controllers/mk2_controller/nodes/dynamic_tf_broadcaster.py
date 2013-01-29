#!/usr/bin/env python

import roslib
roslib.load_manifest('mk2_controller')

import rospy
import tf
import tf.msg
import geometry_msgs.msg
import math

class DynamicTFBroadcaster:

    def __init__(self):
        self.pub_tf = rospy.Publisher("/tf", tf.msg.tfMessage)

        change = 0.0
        while not rospy.is_shutdown():
            # Run this loop at about 10Hz
            rospy.sleep(0.1)

            t = geometry_msgs.msg.TransformStamped()
            t.header.frame_id = "mocap_origin"
            t.header.stamp = rospy.Time.now()
            t.child_frame_id = "gantry_gimbal_base_link"
            #t.transform.translation.x = 0.0
            #t.transform.translation.y = 0.0
            #t.transform.translation.z = 0.0

            #t.transform.rotation.x = 0.0
            t.transform.rotation.y = 3.14
            #t.transform.rotation.z = 0.0
            #t.transform.rotation.w = 0.0

            tfm = tf.msg.tfMessage([t])
            self.pub_tf.publish(tfm)

            change += 0.1

if __name__ == '__main__':
    rospy.init_node('dynamic_tf_broadcaster')
    tfb = DynamicTFBroadcaster()
    rospy.spin()
