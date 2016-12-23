cmake_minimum_required(VERSION 3.4)

project( MultipleImageViewer )

find_package( OpenCV REQUIRED )

file(COPY ../data/lena.jpg ../data/butterfly.jpg ../data/baboon.jpg DESTINATION image)

add_executable( ${PROJECT_NAME} source.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )