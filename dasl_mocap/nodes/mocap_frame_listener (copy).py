#!/usr/bin/env python  
import roslib
roslib.load_manifest('dasl_mocap')
import rospy
import math
import tf
import dasl_mocap.msg
import turtlesim.srv

if __name__ == '__main__':
    rospy.init_node('tf_gantry_trailer_node')

    listener = tf.TransformListener()

    rospy.wait_for_service('spawn')
    spawner = rospy.ServiceProxy('spawn', turtlesim.srv.Spawn)
    spawner(4, 2, 0, 'turtle1')

    turtle_vel = rospy.Publisher('turtle1/command_velocity', dasl_mocap.msg.Velocity)

    rate = rospy.Rate(10.0)
    while not rospy.is_shutdown():
        try:
            (trans,rot) = listener.lookupTransform('/turtle1', '/gantry_base_clone', rospy.Time(0))
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            continue

        angular = 0#4 * math.atan2(trans[1], trans[0])
        linear = 0.5 * math.sqrt(trans[0] ** 2 + trans[1] ** 2)
        turtle_vel.publish(dasl_mocap.msg.Velocity(linear, angular))

        rate.sleep()
