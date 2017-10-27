#!/usr/bin/env python
# coding UTF-8

import yaml
import rospy

from goal_sender_msgs.msg import GoalSequence
from goal_sender_msgs.msg import Waypoint

def read_yaml(path):
    f = open(path, 'r')
    waypoints = yaml.load(f)
    f.close()
    return waypoints

def pub_data():
    pub = rospy.Publisher('goal_sequence', GoalSequence, queue_size=10)
    rospy.init_node('yaml_reader', anonymous=True)
    msg = GoalSequence()
    for waypoint_data in read_yaml(rospy.get_param('~path', 'waypoints.yaml')):
        waypoint = Waypoint(name = waypoint_data.get('name', ""),
                            x = waypoint_data['x'], # required
                            y = waypoint_data['y'], # required
                            radius = waypoint_data['radius'], # required
                            importance = waypoint_data.get('importance', 0),
                            drag = waypoint_data.get('drag', 0))
        msg.waypoints.append(waypoint)
    pub.publish(msg)

if __name__ == '__main__':
    try:
        pub_data()
    except rospy.ROSInterruptException:
        pass
