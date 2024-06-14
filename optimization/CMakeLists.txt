project(optimization)

#采用对象的方式，不要再include_directories()
find_package(Ceres REQUIRED)
add_executable(curveFittingCeres curveFittingCeres.cpp)
target_link_libraries(curveFittingCeres Ceres::ceres)