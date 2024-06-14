project(optimization)

#采用对象的方式，不要再include_directories()
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(curveFittingCeres curveFittingCeres.cpp)
target_link_libraries(curveFittingCeres Ceres::ceres ${OpenCV_LIBS} )
