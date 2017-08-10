cmake_minimum_required(VERSION 2.8.3)
project(fifth_robot_simulator)

 add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  gazebo_ros
  roscpp
  rospy
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES fifth_robot_simulator
#  CATKIN_DEPENDS gazebo roscpp rospy std_msgs
#  DEPENDS system_lib
)
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

