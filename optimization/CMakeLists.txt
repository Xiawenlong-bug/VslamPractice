project(optimization)

#采用对象的方式，不要再include_directories()
find_package(Ceres REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(curveFittingCeres curveFittingCeres.cpp)
target_link_libraries(curveFittingCeres Ceres::ceres ${OpenCV_LIBS} )

add_executable(pose_estimation_3d2d pose_estimation_3d2d.cpp)
target_link_libraries(pose_estimation_3d2d Ceres::ceres ${OpenCV_LIBS} )


# # set casadi include directory
# set(CASADI_INCLUDE_DIR /usr/local/include/casadi)
# # find casadi library
# find_library(CASADI_LIBRARY
#     NAMES casadi
#     HINTS ${CASADI_INCLUDE_DIR}/../lib $ENV{CASADI_PREFIX}/lib)
# if(CASADI_LIBRARY)
#     set(CASADI_LIBRARIES ${CASADI_LIBRARIES} ${CASADI_LIBRARY})
# endif()
# find_package(casadi REQUIRED)
# add_executable(casadiTest casadiTest.cpp)
# target_link_libraries(casadiTest ${CASADI_LIBRARIES})


#several examples of casadi
#https://github.com/zehuilu/Tutorial-on-CasADi-with-CPP?tab=readme-ov-file