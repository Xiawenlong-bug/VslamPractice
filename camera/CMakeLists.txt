project(image_processing)
#set(OpenCV_DIR /home/xwl/opencv-4.5.4/build)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(imageBasics imageBasics.cpp)
target_link_libraries(imageBasics ${OpenCV_LIBS})

add_executable(undistortImages undistortImages.cpp)
target_link_libraries(undistortImages ${OpenCV_LIBS})

