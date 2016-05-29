cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )

project( Match_Template )

file(COPY messi.jpg templ.jpg DESTINATION image)

add_executable( ${PROJECT_NAME} example.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )