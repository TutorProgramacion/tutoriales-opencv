cmake_minimum_required(VERSION 3.4)

project( Tutorial_OpenCV_HDR )

find_package( OpenCV 3.0.0 REQUIRED )

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
file(COPY exposures DESTINATION data)

add_executable( ${PROJECT_NAME} source.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )