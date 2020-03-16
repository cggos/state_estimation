cmake_minimum_required(VERSION 2.8)

project(bundle_adjustment)

add_subdirectory(common)

add_subdirectory(ba_demo_ceres)

add_executable(bal_ceres bal_ceres.cpp)
target_link_libraries(bal_ceres bal_problem parse_cmd ${CERES_LIBRARIES})

add_executable(bal_g2o bal_g2o.cpp)
target_link_libraries(bal_g2o bal_problem parse_cmd ${G2O_LIBS} ${CHOLMOD_LIBRARIES})

add_executable(bal_draw_hessian bal_draw_hessian.cpp)
target_link_libraries(bal_draw_hessian bal_problem parse_cmd ${OpenCV_LIBS})
