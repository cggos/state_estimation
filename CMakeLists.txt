cmake_minimum_required(VERSION 2.8)

project(state_estimation)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_CXX_STANDARD 11)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(Cholmod REQUIRED)
include_directories(${CHOLMOD_INCLUDE_DIR})
# ${CHOLMOD_LIBRARIES}

find_package(Eigen3 REQUIRED)
if(Eigen3_FOUND)
    include_directories(${Eigen3_INCLUDE_DIRS})
endif()

find_package(Sophus)
if(Sophus_FOUND)
    include_directories(${Sophus_INCLUDE_DIRS})
    # link_libraries(Sophus)
endif()
list(APPEND Sophus_LIBRARIES Sophus)

find_package(OpenCV 3 REQUIRED COMPONENTS core imgproc highgui)
if( OpenCV_FOUND )
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

find_package(Ceres REQUIRED)
if(Ceres_FOUND)
    include_directories(${CERES_INCLUDE_DIRS})
endif()

find_package(G2O REQUIRED)
if(G2O_FOUND)
    include_directories(${G2O_INCLUDE_DIRS})
    link_directories(/opt/g2o/lib)
endif()
set(G2O_LIBS g2o_core g2o_stuff g2o_types_sba g2o_solver_csparse g2o_csparse_extension)

# set(G2O_LIBS g2o_cli g2o_ext_freeglut_minimal g2o_simulator g2o_solver_slam2d_linear
#     g2o_types_icp g2o_types_slam2d g2o_types_sba g2o_types_slam3d g2o_core g2o_interface
#     g2o_solver_csparse g2o_solver_structure_only g2o_csparse_extension g2o_opengl_helper g2o_solver_dense
#     g2o_stuff g2o_types_sclam2d g2o_parser g2o_solver_pcg g2o_types_data g2o_types_sim3 cxsparse)

add_subdirectory(optimization_basis)
add_subdirectory(ceres_solver)
add_subdirectory(bundle_adjustment)
add_subdirectory(pose_graph)
