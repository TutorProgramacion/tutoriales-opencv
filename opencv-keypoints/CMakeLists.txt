cmake_minimum_required(VERSION 3.4)

project( Tutorial_Keypoints )

find_package( OpenCV 3.0.0 REQUIRED )

file(COPY images DESTINATION data)

add_executable( ${PROJECT_NAME} source.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )