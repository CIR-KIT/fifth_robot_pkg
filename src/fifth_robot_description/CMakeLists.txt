cmake_minimum_required(VERSION 2.8.3)
project(fifth_robot_description)
packages
find_package(catkin REQUIRED COMPONENTS
  joint_state_publisher
  robot_state_publisher
  rviz
  xacro
)

catkin_package(
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

