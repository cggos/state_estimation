cmake_minimum_required(VERSION 2.8)
project(btest)

add_executable(btest Main.cpp BSplineBasisFunction.cpp)
target_link_libraries(btest osg osgDB osgGA osgViewer)

