cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )

project( Histogramas )

file(COPY lena.jpg DESTINATION image)

add_executable( ${PROJECT_NAME} histogram.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )