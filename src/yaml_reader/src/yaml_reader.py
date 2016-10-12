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
    if waypoints["start"]["inportance"] == 1:
       print '%s is Passing point.' % 'start'
    if waypoints["gate"]["inportance"] == 2:
       print '%s is Important Passing point.' % 'gate'
    if waypoints["slope"]["inportance"] == 3:
       print '%s is Dangerous point.' % 'slope'
    if waypoints["goal"]["inportance"] == 4:
       print '%s is destination.' % 'goal'

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
