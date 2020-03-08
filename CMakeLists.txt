cmake_minimum_required(VERSION 2.8)

project(slam_optimization)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_CXX_STANDARD 11)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(Eigen3 REQUIRED)
if(Eigen3_FOUND)
    include_directories( ${Eigen3_INCLUDE_DIRS} )
endif()

find_package( Sophus REQUIRED )
if(Sophus_FOUND)
    include_directories( ${Sophus_INCLUDE_DIRS} )
    link_libraries(Sophus)
endif()

find_package(G2O REQUIRED)
if(G2O_FOUND)
    include_directories(${G2O_INCLUDE_DIRS})
    link_directories(/opt/g2o/lib)
endif()

add_subdirectory( optimization_basis )

add_subdirectory( bundle_adjustment )
add_subdirectory( pose_graph )

add_subdirectory( ceres_solver )
