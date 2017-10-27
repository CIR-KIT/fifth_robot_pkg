#!/usr/bin/env python
# coding UTF-8

import yaml
import rospy

from std_msgs.msg import String

FILEIN_DICT = "waypoints.yaml" #Rename!

f = open(FILEIN_DICT, 'r')
waypoints = yaml.load(f)
f.close()

def pubData():
    pub = rospy.Publisher('data', String, queue_size=10)
    rospy.init_node('pubData', anonymous=True)
    r = rospy.Rate(10)
    while not rospy.is_shutdown():
        pub.publish(waypoints)
        r.sleep()

if __name__ == '__main__':
    try:
        pubData()
    except rospy.ROSInterruptException:
        pass
