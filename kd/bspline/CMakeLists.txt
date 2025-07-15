cmake_minimum_required(VERSION 2.8)
project(bspline)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(fmt REQUIRED)
if(fmt_FOUND)
  message("-- FOUND fmt")
endif()

find_package(Sophus REQUIRED)
if(Sophus_FOUND)
  include_directories(${Sophus_INCLUDE_DIRS})
  set(Sophus_LIBRARIES fmt) # sophus
endif()

find_package(Pangolin)
include_directories(${Pangolin_INCLUDE_DIRS})

add_executable(bspline bspline.cpp)
target_link_libraries(bspline ${Sophus_LIBRARIES})

add_executable(bspline_ov bspline_ov.cpp BsplineSE3.cpp Simulator.cpp utils/print.cpp)
