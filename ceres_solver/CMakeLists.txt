cmake_minimum_required(VERSION 2.8)

project(ceres_solver_demos)

set(CMAKE_CXX_STANDARD 11)

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})
link_libraries(${CERES_LIBRARIES})

add_executable(helloworld helloworld.cc)
target_link_libraries(helloworld glog)