SET(G2O_LIBS g2o_cli g2o_ext_freeglut_minimal g2o_simulator g2o_solver_slam2d_linear
    g2o_types_icp g2o_types_slam2d g2o_types_sba g2o_types_slam3d g2o_core g2o_interface
    g2o_solver_csparse g2o_solver_structure_only g2o_csparse_extension g2o_opengl_helper g2o_solver_dense
    g2o_stuff g2o_types_sclam2d g2o_parser g2o_solver_pcg g2o_types_data g2o_types_sim3 cxsparse )

find_package(Cholmod REQUIRED)
include_directories(${CHOLMOD_INCLUDE_DIR} )

include_directories(ceres ../common)

add_executable(ba_g2o g2o_bundle.cpp)
target_link_libraries(ba_g2o ${G2O_LIBS} ${CHOLMOD_LIBRARIES} BALProblem ParseCmd)
