project(vslam)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
#opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#pangolin
#Pangolin是一个轻量、便携的管理OpenGl显示、交互和提取的视频输入的快速开发库。其核心是一个简单的OpenGl视口管理器，它能帮助模块化3D可视化，不增加复杂性，提供一个先进且直接的3D导航处理器。
#Pangolin也提供一个操作程序的机制，这个机制通过配置文件和UI集成，有一个灵活的实时绘图仪，用于可视图形图像数据。
find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})

# Sophus
find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIRS})

#Ceres
#采用对象的方式，不要再include_directories()
#target_link_libraries(xxx Ceres::ceres)
find_package(Ceres REQUIRED)


# glog
find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})

# gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# gflags
find_package(GFlags REQUIRED)
include_directories(${GFLAGS_INCLUDE_DIRS})

# csparse
find_package(CSparse REQUIRED)
include_directories(${CSPARSE_INCLUDE_DIR})

set(THIRD_PARTY_LIBS ${OpenCV_LIBS} 
                        ${Sophus_LIBRARIES}
                        ${Pangolin_LIBRARIES}
                        Ceres::ceres
                        ${GTEST_BOTH_LIBRARIES}
                        ${GLOG_LIBRARIES}
                        ${GFLAGS_LIBRARIES}
                        pthread
                        ${CSPARSE_LIBRARY}
    )


include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(app)