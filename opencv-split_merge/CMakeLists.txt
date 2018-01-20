cmake_minimum_required(VERSION 3.0)

project( Tutorial_OpenCV )

find_package( OpenCV 3.0.0 REQUIRED )

file(COPY rgb.png DESTINATION image)

add_executable( ${PROJECT_NAME} source.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )