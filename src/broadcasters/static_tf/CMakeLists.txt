cmake_minimum_required(VERSION 2.8.3)
project(static_tf)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES static_tf
  CATKIN_DEPENDS roscpp tf
#  DEPENDS system_lib
)

###########
## Build ##
###########

# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)


## Declare a C++ executable
add_executable(static_tf_node src/static_tf_node.cpp)


## Specify libraries to link a library or executable target against
target_link_libraries(static_tf_node
  ${catkin_LIBRARIES}
)
