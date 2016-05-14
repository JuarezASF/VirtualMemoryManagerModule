cmake_minimum_required(VERSION 3.3)
project(VirtualMemoryManager)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(src)
add_subdirectory(src)
add_subdirectory(test)

