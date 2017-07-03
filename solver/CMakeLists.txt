cmake_minimum_required(VERSION 3.0)
if (POLICY CMP0020)
cmake_policy(SET CMP0020 OLD)
endif (POLICY CMP0020)

project(solver CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++14 -static")
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(test)
