cmake_minimum_required( VERSION 2.8 )
project( eigen_demos )

set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

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

find_package(Ceres REQUIRED)
if(Ceres_FOUND)
  include_directories(${CERES_INCLUDE_DIRS})
endif()

find_package( Pangolin )
include_directories( ${Pangolin_INCLUDE_DIRS} )

add_executable( eigen_geometry eigen_geometry.cpp )

add_executable( eigen_geometry_quickref eigen_geometry_quickref.cpp )

add_executable( eigen_geometry_visualize eigen_geometry_visualize.cpp )
target_link_libraries( eigen_geometry_visualize ${Pangolin_LIBRARIES} )

add_executable( eigen_sophus eigen_sophus.cpp )
target_link_libraries( eigen_sophus ${Sophus_LIBRARIES} )

add_executable( eigen_rotation eigen_rotation.cpp)
target_link_libraries( eigen_rotation ${Sophus_LIBRARIES} ${CERES_LIBRARIES} )

add_executable( eigen_quaternion_slerp eigen_quaternion_slerp.cpp )
