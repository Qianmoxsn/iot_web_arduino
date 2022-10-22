cmake_minimum_required(VERSION 3.23)
project(udp C)

set(CMAKE_C_STANDARD 99)

add_executable(udp main.c)
target_link_libraries(udp ws2_32)
