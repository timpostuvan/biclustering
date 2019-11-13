cmake_minimum_required(VERSION 2.6)
project (biclustering)
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

include_directories(src include)

add_library(lib
	include/matrix.h
	include/cost_functions.h
	src/matrix.cpp
	src/cost_functions.cpp
	src/fixed_rows.cpp
)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE lib)