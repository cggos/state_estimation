add_executable(ba_gauss_newton ba_gauss_newton.cpp)

add_executable(ba_gn_ceres ba_gn_ceres.cpp)
target_link_libraries(ba_gn_ceres ${CERES_LIBRARIES})

add_executable(ba_gn_g2o ba_gn_g2o.cpp)
target_link_libraries(ba_gn_g2o ${G2O_LIBS} ${CHOLMOD_LIBRARIES})