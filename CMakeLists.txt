cmake_minimum_required(VERSION 3.16)
project(web_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(web_server 
src/main.c 
src/tcp.c 
src/http.c
src/route.c
src/config.c
)

add_library(mylib STATIC IMPORTED)
set_target_properties(mylib PROPERTIES
	IMPORTED_LOCATION	"${CMAKE_SOURCE_DIR}/lib/libcjson.a"
	INTERFACE_INCLUDE_DIRECTORIES	"${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(web_server PRIVATE mylib)
target_compile_options(web_server PRIVATE -g -Wall -Wextra -Wpedantic)
