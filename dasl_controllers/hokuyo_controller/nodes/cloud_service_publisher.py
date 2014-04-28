#!/usr/bin/env python
import roslib; roslib.load_manifest('laser_assembler')
import rospy; from laser_assembler.srv import *
from sensor_msgs.msg import PointCloud2

rospy.init_node("hokuyo_cloud")
pub_cloud = rospy.Publisher("hokuyo/points",PointCloud2)
while not rospy.is_shutdown():
	rospy.wait_for_service("assemble_scans2")
	try:
		assemble_scans = rospy.ServiceProxy('assemble_scans2', AssembleScans2)
		resp = assemble_scans(rospy.Time(0,0), rospy.get_rostime())
		pub_cloud.publish(resp.cloud)
	except rospy.ServiceException, e:
		print "Service call failed: %s"%e
