#!/usr/bin/env python  
import roslib
roslib.load_manifest('dasl_mocap')
import rospy

import tf
import dasl_mocap.msg

def handle_turtle_pose(msg, turtlename):
    br = tf.TransformBroadcaster()
    br.sendTransform((msg.x, msg.y, 0),
                     tf.transformations.quaternion_from_euler(0, 0, msg.theta),
                     rospy.Time.now(),
                     turtlename,
                     "mocap_origin")

if __name__ == '__main__':
    rospy.init_node('mocap_tf_broadcaster')
    turtlename = rospy.get_param('~turtle')
    rospy.Subscriber('/%s/pose' % turtlename,
                     dasl_mocap.msg.Pose,
                     handle_turtle_pose,
                     turtlename)
    rospy.spin()
