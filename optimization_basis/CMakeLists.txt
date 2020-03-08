find_package( Ceres REQUIRED )
include_directories( ${CERES_INCLUDE_DIRS} )

find_package( OpenCV )
if( OpenCV_FOUND )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
endif()

add_executable( curve_fitting_gn curve_fitting_gn.cpp )
target_link_libraries( curve_fitting_gn ${OpenCV_LIBS} )

add_executable( curve_fitting_ceres curve_fitting_ceres.cpp )
target_link_libraries( curve_fitting_ceres ${CERES_LIBRARIES} ${OpenCV_LIBS} )

add_executable( curve_fitting_g2o curve_fitting_g2o.cpp )
target_link_libraries( curve_fitting_g2o ${OpenCV_LIBS} g2o_core g2o_stuff )
