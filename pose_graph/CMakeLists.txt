cmake_minimum_required(VERSION 2.8)

project(pose_graph)

find_package(GTSAMCMakeTools)
find_package(GTSAM REQUIRED)
include_directories(${GTSAM_INCLUDE_DIR})

add_executable(pose_graph_g2o_SE3 pose_graph_g2o_SE3.cpp )
target_link_libraries(pose_graph_g2o_SE3 g2o_core g2o_stuff g2o_types_slam3d ${CHOLMOD_LIBRARIES})

add_executable(pose_graph_g2o_lie pose_graph_g2o_lie_algebra.cpp)
target_link_libraries(pose_graph_g2o_lie g2o_core g2o_stuff ${CHOLMOD_LIBRARIES} ${Sophus_LIBRARIES})

add_executable(pose_graph_gtsam pose_graph_gtsam.cpp)
target_link_libraries(pose_graph_gtsam gtsam)
