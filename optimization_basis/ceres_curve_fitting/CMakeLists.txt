find_package( Ceres REQUIRED )
include_directories( ${CERES_INCLUDE_DIRS} )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_DIRS} )

add_executable( ceres_curve_fitting ceres_curve_fitting.cpp )
target_link_libraries( ceres_curve_fitting ${CERES_LIBRARIES} ${OpenCV_LIBS} )
