cmake_minimum_required(VERSION 3.1)
project(eigen)

option(WITH_SIMD "with SIMD" ON)
option(WITH_OMP "with OMP" ON)
option(WITH_EIGEN_FAST "" ON)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-w)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") # !!!!!
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -S")

if(WITH_SIMD)
  if(DEFINED ENV{ARM_ARCHITECTURE})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -mfpu=neon -march=armv7-a")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse4.2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -mfma") # !!!!!
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")
  endif()
endif()

if(WITH_OMP)
  find_package(OpenMP) # !!!!!
  if (OPENMP_FOUND)
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
      set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
endif()

if(WITH_EIGEN_FAST)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEIGEN_FAST_MATH -DEIGEN_NO_DEBUG")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEIGEN_DONT_VECTORIZE")
endif()

# set(Eigen3_DIR "/opt/eigen-337/share/eigen3/cmake/")
find_package(Eigen3 REQUIRED)
if(Eigen3_FOUND)
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui imgcodecs)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable(eigen_matrix eigen_matrix.cpp)

add_executable(eigen_tensor eigen_tensor.cpp)

add_executable(eigen_simd_test eigen_simd_test.cpp)
# target_compile_options(eigen_simd_test PUBLIC "-S")

add_executable(eigen_matrix_mul eigen_matrix_mul.cpp)
target_link_libraries(eigen_matrix_mul ${OpenCV_LIBS})

add_executable(givens_rotation givens_rotation.cpp)
