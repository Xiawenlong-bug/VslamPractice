add_executable(run_kitti_stereo run_kitti_stereo.cpp)
#myslam是项目源文件生成的库
target_link_libraries(run_kitti_stereo ${THIRD_PARTY_LIBS} myslam)