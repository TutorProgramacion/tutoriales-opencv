cmake_minimum_required(VERSION 3.4)

project( PixelateImage )

find_package( OpenCV REQUIRED )

file(COPY ../data/lena.jpg DESTINATION image)

add_executable( ${PROJECT_NAME} pixelate.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )