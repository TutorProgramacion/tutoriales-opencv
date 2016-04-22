cmake_minimum_required(VERSION 2.8)

project( Restauracion_Inpaint )

find_package( OpenCV REQUIRED )

file(COPY baboon.jpg DESTINATION data)

add_executable( Restauracion_Inpaint restauracion.cpp )
target_link_libraries( Restauracion_Inpaint ${OpenCV_LIBS} )