project(dense_mapping)
set(CMAKE_CXX_STANDARD 17)

find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# eigen 
include_directories("/usr/include/eigen3/")
# octomap 
find_package(octomap REQUIRED)
include_directories(${OCTOMAP_INCLUDE_DIRS})


add_executable(pcl_mapping pcl_mapping.cpp)
target_link_libraries(pcl_mapping ${PCL_LIBRARIES} ${OpenCV_LIBS})  
