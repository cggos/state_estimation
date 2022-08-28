cmake_minimum_required(VERSION 2.8)

project(ceres_solver_demos)

set(CMAKE_CXX_STANDARD 11)

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})
link_libraries(${CERES_LIBRARIES})

link_libraries(glog)

add_executable(ceres_demo_01 ceres_demo_01.cc)

add_executable(ceres_demo_02 ceres_demo_02.cc)

add_executable(ceres_demo_03 ceres_demo_03.cc)

add_executable(ceres_demo_04 ceres_demo_04.cc)
