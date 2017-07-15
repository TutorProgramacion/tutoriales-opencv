cmake_minimum_required(VERSION 3.4)

project( Tutorial_CVUI )

find_package( OpenCV 3.0.0 REQUIRED )

file(COPY ../data/lena.jpg DESTINATION imagen)

add_executable( ${PROJECT_NAME} cvui_test.cpp cvui.h)

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )