cmake_minimum_required(VERSION 3.24)
project(TP4)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
link_libraries(ws2_32 wsock32)

add_executable(TP4 FindHost.cpp)
