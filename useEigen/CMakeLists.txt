project(useEigen)
#include_directories("usr/include/eigen3")
add_executable(eigenMatrix eigenMatrix.cpp)
add_executable(eigenGeometry eigenGeometry.cpp)

find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})
add_executable(plotTrajectory plotTrajectory.cpp)
target_link_libraries(plotTrajectory ${Pangolin_LIBRARIES})

add_subdirectory(eigenhomework)