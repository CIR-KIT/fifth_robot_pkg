cmake_minimum_required(VERSION 2.8.3)
project(waypoints_reader)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES yamlReader
#  CATKIN_DEPENDS rospy std_msgs
#  DEPENDS system_lib
)
