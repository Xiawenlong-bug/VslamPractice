project(bundle_adjustment_project)


#采用对象的方式，不要再include_directories()
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Sophus REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(BAL_common common.cpp)

add_executable(bundle_adjustment_ceres bundle_adjustment_ceres.cpp)
target_link_libraries(bundle_adjustment_ceres Ceres::ceres ${OpenCV_LIBS} BAL_common)
