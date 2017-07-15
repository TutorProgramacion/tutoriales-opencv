cmake_minimum_required(VERSION 3.4)

project( OpticalFlow_Farneback )

find_package( OpenCV 3.0.0 REQUIRED )

add_executable( ${PROJECT_NAME} optical_flow_farneback.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )