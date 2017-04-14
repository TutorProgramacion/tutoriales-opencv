cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )

project( Match_Template )

file(COPY images DESTINATION data)

add_executable( ${PROJECT_NAME} example.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )