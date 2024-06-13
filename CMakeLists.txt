cmake_minimum_required(VERSION 2.8)

project(VSlamPractice)

add_executable(helloSLAM helloSLAM.cpp)


include_directories("/usr/include/eigen3")
add_subdirectory(useEigen)

