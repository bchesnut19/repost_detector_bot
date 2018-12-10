cmake_minimum_required(VERSION 2.8.12)
project(test)



add_subdirectory(/usr/include/dlib dlib_build)

add_executable(compar.x image_comparator.cpp)
target_link_libraries(compar.x dlib::dlib)
