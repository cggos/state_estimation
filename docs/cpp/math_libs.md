# Mathematical Libraries

Mathematical libraries used in development and those usages with CMake.

* [boost Math Toolkit](https://www.boost.org/doc/libs/1_66_0/libs/math/doc/html/index.html)
* [Netlib](http://www.netlib.org/) Repository at UTK and ORNL
* [CodeCogs](https://www.codecogs.com/): Open Source Scientific Library (C/C++, .NET, Excel)
* [GSL](https://www.gnu.org/software/gsl/gsl.html) (GNU Scientific Library): a numerical library for C and C++ programmers
    - [GSL Shell](https://savannah.nongnu.org/projects/gsl-shell/) offers an interactive command-line interface that gives access to GSL collection of mathematical functions
* [C++11 Fixed Point Arithmetic Library](https://embeddedartistry.com/blog/2017/8/25/c11-fixed-point-arithemetic-library)

-----

## Basic Linear Algebra

* [Boosting numpy: Why BLAS Matters](https://markus-beuckelmann.de/blog/boosting-numpy-blas.html)

### BLAS

The [BLAS](http://www.netlib.org/blas/) (Basic Linear Algebra Subprograms) are routines that provide standard building blocks for **performing basic vector and matrix operations**. The Level 1 BLAS perform scalar, vector and vector-vector operations, the Level 2 BLAS perform matrix-vector operations, and the Level 3 BLAS perform matrix-matrix operations. Because the BLAS are efficient, portable, and widely available, they are commonly used in the development of high quality linear algebra software, LAPACK for example.

### LAPACK

[LAPACK](http://www.netlib.org/lapack/) is written in Fortran 90 and provides routines for **solving systems of simultaneous linear equations, least-squares solutions of linear systems of equations, eigenvalue problems, and singular value problems**.

```sh
find_package(LAPACK REQUIRED)
if(LAPACK_FOUND)
    message(STATUS "LAPACK Libraries: ")
    foreach (lib ${LAPACK_LIBRARIES})
        message(STATUS "  " ${lib})
    endforeach()
    link_libraries( ${LAPACK_LIBRARIES} )
endif()
```

### ATLAS

The [ATLAS](http://math-atlas.sourceforge.net/) (Automatically Tuned Linear Algebra Software) project is an ongoing research effort focusing on applying empirical techniques in order to provide portable performance. At present, it provides C and Fortran77 interfaces to a portably efficient BLAS implementation, as well as a few routines from LAPACK.

### OpenBLAS

[OpenBLAS](http://www.openblas.net/) is **an optimized BLAS library** based on GotoBLAS2 1.13 BSD version.

### SuiteSparse

[SuiteSparse](http://faculty.cse.tamu.edu/davis/suitesparse.html): A Suite of Sparse Matrix Software.

### Eigen

[Eigen](http://eigen.tuxfamily.org) is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.

```sh
find_package(Eigen3 REQUIRED)
if(EIGEN3_FOUND)
    include_directories( ${EIGEN3_INCLUDE_DIR} )
endif()
```

* [Eigen Library Tutorial](https://www.cc.gatech.edu/classes/AY2015/cs4496_spring/Eigen.html)

### Matrix Template Library (MTL2/MTL4)

The Matrix Template Library 4 (MTL4) is a development library for scientific computing that combines high productivity with high performance in the execution.

### Blitz++

[Blitz++](https://sourceforge.net/projects/blitz/) is a C++ class library for scientific computing which provides performance on par with Fortran 77/90. It uses template techniques to achieve high performance. Blitz++ provides dense arrays and vectors, random number generators, and small vectors (useful for representing multicomponent or vector fields).

### CLBlast

[CLBlast](https://github.com/CNugteren/CLBlast) is a modern, lightweight, performant and tunable OpenCL BLAS library written in C++11.

### GMP

[GMP](https://gmplib.org/) is a free library for arbitrary precision arithmetic, operating on signed integers, rational numbers, and floating-point numbers.


## Lie theory

### TooN

TooN: Tom's Object-oriented numerics library.   
The TooN library is a set of C++ header files which provide basic linear algebra facilities.

```sh
# TooN
# Require linking the LAPACK library
```

### Sophus

C++ implementation of Lie Groups using Eigen commonly used for 2d and 3d geometric problems (i.e. for Computer Vision or Robotics applications).

```sh
find_package( Sophus REQUIRED )
if(Sophus_FOUND)
    include_directories( ${Sophus_INCLUDE_DIRS} )
    link_libraries( ${Sophus_LIBRARIES} )
endif()
```

### manif

[manif](https://github.com/artivis/manif) is a header-only c++11 Lie theory library for state-estimation targeted at robotics applications

## DFT

### FFTW

[FFTW](http://www.fftw.org/) is a C subroutine library for computing the discrete Fourier transform (DFT) in one or more dimensions, of arbitrary input size, and of both real and complex data (as well as of even/odd data, i.e. the discrete cosine/sine transforms or DCT/DST).

## Optimization

### Ceres Solver

[Ceres Solver](http://ceres-solver.org/) is an open source C++ library for modeling and solving large, complicated optimization problems. It can be used to solve Non-linear Least Squares problems with bounds constraints and general unconstrained optimization problems.

```sh
find_package( Ceres REQUIRED )
if(Ceres_FOUND)
    include_directories( ${CERES_INCLUDE_DIRS} )
    link_libraries( ${CERES_LIBRARIES} )
endif()
```

### G2O

[g2o](https://openslam-org.github.io/g2o.html): A General Framework for Graph Optimization

```sh
find_package( G2O REQUIRED )
if(G2O_FOUND)
    include_directories( ${G2O_INCLUDE_DIRS} )
    link_libraries( g2o_core g2o_stuff g2o_types_sba )
endif()
```

### ensmallen

[ensmallen](http://ensmallen.org/): flexible C++ library for efficient mathematical optimization

## Search

### ANN

[ANN](https://www.cs.umd.edu/~mount/ANN/): A Library for Approximate Nearest Neighbor Searching

### FLANN

[FLANN](https://www.cs.ubc.ca/research/flann/) is a library for performing fast approximate nearest neighbor searches in high dimensional spaces.

### Annoy

[Annoy](https://github.com/spotify/annoy) (Approximate Nearest Neighbors Oh Yeah) is a C++ library with Python bindings to search for points in space that are close to a given query point.
