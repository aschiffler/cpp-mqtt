cmake_minimum_required(VERSION 3.10)
project(cpp-mqtt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static") 

add_executable(cpp-mqtt main.cpp)