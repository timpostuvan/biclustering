cmake_minimum_required(VERSION 2.6)
project (biclustering)
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

include_directories(src include)

add_library(lib
	include/matrix.h
	include/cost_functions.h
	include/fixed_rows.h
	include/solution.h
	include/2D.h
	src/matrix.cpp
	src/cost_functions.cpp
	src/fixed_rows.cpp
	src/solution.cpp
	src/2D.cpp
)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE lib)

add_executable(distance correlation/distance.cpp)
target_link_libraries(distance PRIVATE lib)

add_executable(approximation correlation/approximation.cpp)
target_link_libraries(approximation PRIVATE lib)

add_executable(hill_climbing 2D/hill_climbing.cpp)
target_link_libraries(hill_climbing PRIVATE lib)

add_executable(optimal_brute linear-programming/optimal_brute.cpp)
target_link_libraries(optimal_brute PRIVATE lib)