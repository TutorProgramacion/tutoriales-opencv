cmake_minimum_required(VERSION 3.7)

project( "OpenCV-Filtro_Twirl" )

find_package( OpenCV REQUIRED )

file(COPY ../data/lena.jpg DESTINATION data)

add_executable( ${PROJECT_NAME} twirl.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )