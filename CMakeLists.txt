cmake_minimum_required(VERSION 3.10)
project(NDIlib_Find)
set(CMAKE_CXX_STANDARD 14)
include_directories(../../../include)
link_directories(../../../lib/x86_64-linux-gnu)
add_executable(NDIlib_Find NDIlib_Find.cpp)
target_link_libraries(NDIlib_Find libndi.so)
