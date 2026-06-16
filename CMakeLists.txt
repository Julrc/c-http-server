cmake_minimum_required(VERSION 3.16)
project(tcp_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(server src/main.c src/tcp.c)

target_include_directories(server PRIVATE include)
