cmake_minimum_required(VERSION 3.16)
project(web_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(web_server 
src/main.c 
src/config.c
src/http.c
src/route.c
src/tcp.c 
src/worker.c
src/arena.c
src/file.c
thread_pool/thread_pool.c
)

add_library(mylib STATIC IMPORTED)
set_target_properties(mylib PROPERTIES
	IMPORTED_LOCATION	"${CMAKE_SOURCE_DIR}/lib/libcjson.a"
	INTERFACE_INCLUDE_DIRECTORIES	"${CMAKE_SOURCE_DIR}/include"
)

target_include_directories(web_server PRIVATE
include
thread_pool
)


find_package(Threads REQUIRED)

target_link_libraries(web_server PRIVATE mylib Threads::Threads)
target_compile_options(web_server PRIVATE -g -Wall -Wextra -Wpedantic -fsanitize=address -fno-omit-frame-pointer)
target_link_options(web_server PRIVATE -fsanitize=address)

