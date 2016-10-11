#!/usr/bin/env python
import yaml
import rospy


from std_msgs.msg import _Float32


FILEIN_DICT = "waypoints.yaml"

f = open(FILEIN_DICT, 'r')
waypoints = yaml.load(f)
f.close()

#def checkstuck():
#def findnum(num):
#def findwaypoint():
#def findimportance():
#def findradius():
#def finddrug():
#def calc_angle():
#def mod_angle():
#def change_waypoint():
#def goal():

def pubGoal():
    pub = rospy.Publisher('goal', float)#quie size?
    rospy.init_node('pubGoal', anonymous=True)
    r = rospy.Rate(10) # 10hz??ok??
    while not rospy.is_shutdown():
        pub.publish(goal)
        r.sleep()

if __name__ == '__main__':
    try:
        pubGoal()
    except rospy.ROSInterruptException: pass
