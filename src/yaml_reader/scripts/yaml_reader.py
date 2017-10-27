#!/usr/bin/env python
# coding UTF-8

import yaml
import rospy

from std_msgs.msg import String

def pubData():
    pub = rospy.Publisher('data', String, queue_size=10)
    rospy.init_node('pubData', anonymous=True)
    f = open(rospy.get_param('path', 'waypoints.yaml'), 'r')
    waypoints = yaml.load(f)
    f.close()
    r = rospy.Rate(10)
    while not rospy.is_shutdown():
        pub.publish(waypoints)
        r.sleep()

if __name__ == '__main__':
    try:
        pubData()
    except rospy.ROSInterruptException:
        pass
