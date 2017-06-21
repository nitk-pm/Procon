cmake_minimum_required(VERSION 3.0)
if (POLICY CMP0020)
cmake_policy(SET CMP0020 OLD)
endif (POLICY CMP0020)

project(solver CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-Wall -O2 -DQT_MESSAGELOGCONTEXT -std=c++14 -static")
set(CMAKE_BUILD_TYPE Release)

file(GLOB SOURCE_FILE src/*/*.cpp src/*/*.h)

add_executable(
    solver
    ${SOURCE_FILE}
)

target_include_directories(solver PRIVATE src)
