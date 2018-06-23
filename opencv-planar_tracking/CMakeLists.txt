cmake_minimum_required(VERSION 3.4)

project( Tutorial_Detect_Planar_Object )

find_package( OpenCV 3.0.0 REQUIRED )

file(COPY box.png DESTINATION image)
file(COPY box_in_scene.png DESTINATION image)

add_executable( ${PROJECT_NAME} main.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )