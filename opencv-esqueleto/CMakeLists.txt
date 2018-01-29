cmake_minimum_required(VERSION 3.0)

project( Tutorial_OpenCV_Esqueleto )

find_package( OpenCV 3.0.0 REQUIRED )

file(COPY star.jpg DESTINATION data)

add_executable( ${PROJECT_NAME} source.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )