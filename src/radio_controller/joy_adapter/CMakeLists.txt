cmake_minimum_required(VERSION 2.8.3)
project(joy_adapter)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  joy
  roscpp
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES joy_adapter
  CATKIN_DEPENDS geometry_msgs joy roscpp
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(joy_adapter_node src/joy_adapter_node.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(joy_adapter_node
  ${catkin_LIBRARIES}
)
