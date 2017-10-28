cmake_minimum_required(VERSION 2.8.3)
project(goal_sender_msgs)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  message_generation
)

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  Waypoint.msg
  GoalSequence.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  ApplyGoals.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  geometry_msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  CATKIN_DEPENDS geometry_msgs message_runtime
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)
