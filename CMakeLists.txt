cmake_minimum_required(VERSION 3.10)
project(cpp-mqtt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static") 
#set(PAHO_MQTT_C_LIBRARIES "paho.mqtt.c/lib")
#set(PAHO_MQTT_C_INCLUDE_DIRS "paho.mqtt.c")

include_directories(paho.mqtt.c)

add_subdirectory(paho.mqtt.c)
#add_subdirectory(paho.mqtt.cpp)

add_executable(cpp-mqtt main.cpp)
