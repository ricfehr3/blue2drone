cmake_minimum_required(VERSION 2.8.3)
project(bluetooth_read)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(blue_read src/blue_read.cpp)
target_link_libraries(blue_read ${catkin_LIBRARIES})
target_link_libraries(blue_read serial)
add_dependencies(blue_read blue_read_generate_messages_cpp)
