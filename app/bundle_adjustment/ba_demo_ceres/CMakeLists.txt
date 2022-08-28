cmake_minimum_required(VERSION 2.8)

project(ba_demo_ceres)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS} )

add_executable(ba_demo_ceres ba_demo.cpp parametersse3.cpp)
target_link_libraries(ba_demo_ceres ${CERES_LIBRARIES})

add_executable(ba_6d ba_demo_6d.cpp parametersse3.cpp)
target_link_libraries(ba_6d ${CERES_LIBRARIES})
