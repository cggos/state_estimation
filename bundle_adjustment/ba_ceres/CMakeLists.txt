find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

include_directories(../common)

add_executable(ba_ceres ceresBundle.cpp)
target_link_libraries(ba_ceres ${CERES_LIBRARIES} BALProblem ParseCmd  )
