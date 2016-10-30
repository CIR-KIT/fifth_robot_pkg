cmake_minimum_required(VERSION 2.8.3)
project(teleop_master)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES teleop_master
  CATKIN_DEPENDS geometry_msgs roscpp
#  DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(teleop_master_node src/teleop_master_node.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(teleop_master_node
  ${catkin_LIBRARIES}
)

