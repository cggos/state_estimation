cmake_minimum_required(VERSION 2.8)

project(slam_optimization)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_CXX_STANDARD 11)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(Eigen3 REQUIRED)
include_directories( ${Eigen3_INCLUDE_DIRS} )
# include_directories("/opt/eigen-3.3.3/include/eigen3")

find_package( Sophus REQUIRED )
include_directories( ${Sophus_INCLUDE_DIRS} )
link_libraries(Sophus)

include_directories(/opt/g2o_gx/include/)
link_directories(/opt/g2o_gx/lib/)

add_subdirectory( optimization_basis/gauss_newton )
add_subdirectory( optimization_basis/g2o_curve_fitting )
add_subdirectory( optimization_basis/ceres_curve_fitting )

add_subdirectory( bundle_adjustment/common )
add_subdirectory( bundle_adjustment/ba_gauss_newton )
add_subdirectory( bundle_adjustment/ba_g2o )
add_subdirectory( bundle_adjustment/ba_ceres )

add_subdirectory( pose_graph )
