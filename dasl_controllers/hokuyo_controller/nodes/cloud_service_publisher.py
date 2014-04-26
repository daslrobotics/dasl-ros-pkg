#!/usr/bin/env python
import roslib; roslib.load_manifest('laser_assembler')
import rospy; from laser_assembler.srv import *
from sensor_msgs.msg import PointCloud

rospy.init_node("hokuyo_cloud")
pub_cloud = rospy.Publisher("hokuyo/points",PointCloud)
while not rospy.is_shutdown():
	rospy.wait_for_service("assemble_scans")
	try:
		assemble_scans = rospy.ServiceProxy('assemble_scans', AssembleScans)
		resp = assemble_scans(rospy.Time(0,0), rospy.get_rostime())
		print "Got cloud with %u points" % len(resp.cloud.points)
		pub_cloud.publish(resp.cloud)
	except rospy.ServiceException, e:
		print "Service call failed: %s"%e
