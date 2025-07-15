cmake_minimum_required(VERSION 2.8.3)
project(ros_tf)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  tf
  tf_conversions
  turtlesim
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ros_tf
#  CATKIN_DEPENDS roscpp rospy tf turtlesim
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(turtle_tf_broadcaster src/turtle_tf_broadcaster.cpp)
target_link_libraries(turtle_tf_broadcaster ${catkin_LIBRARIES})

add_executable(turtle_tf_listener src/turtle_tf_listener.cpp)
target_link_libraries(turtle_tf_listener ${catkin_LIBRARIES})

add_executable(frame_tf_broadcaster src/frame_tf_broadcaster.cpp)
target_link_libraries(frame_tf_broadcaster ${catkin_LIBRARIES})

add_executable(ros_tf_test src/ros_tf_test.cpp)
target_link_libraries(ros_tf_test ${catkin_LIBRARIES})

add_executable(show_path src/show_path.cpp)
target_link_libraries(show_path ${catkin_LIBRARIES})