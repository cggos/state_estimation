
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})
link_libraries(${CERES_LIBRARIES})

add_executable(ba_gauss_newton ba_gauss_newton.cpp)

add_executable(ba_gn_ceres ba_gn_ceres.cpp)