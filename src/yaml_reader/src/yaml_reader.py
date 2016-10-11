#!/usr/bin/env python
# coding UTF-8

import yaml
import rospy

from std_msgs.msg import String

FILEIN_DICT = "waypoint.yaml"#Rename!

f = open(FILEIN_DICT, 'r')
waypoints = yaml.load(f)
f.close()


def checkImportance():
    if waypoints["wps"][0]["inportance"] == 1:
       print '%s is Passing point.' % (waypoints["wps"][0]["name"])
    if waypoints["wps"][1]["inportance"] == 2:
       print '%s is Important Passing point.' % (waypoints["wps"][1]["name"])
    if waypoints["wps"][2]["inportance"] == 3:
       print '%s is Dangerous point.' % (waypoints["wps"][2]["name"])
    if waypoints["wps"][3]["inportance"] == 4:
       print '%s is destination.' % (waypoints["wps"][3]["name"])

def pubData():
    pub = rospy.Publisher('data', String, queue_size=10)
    rospy.init_node('pubData', anonymous=True)
    r = rospy.Rate(10)
    while not rospy.is_shutdown():
        pub.publish(data)
        r.sleep()

if __name__ == '__main__':
    try:
        pubData()
    except rospy.ROSInterruptException: pass
