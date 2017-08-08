
add_executable(geometry_test geometry_test.cpp)
target_link_libraries(
    geometry_test
    libgtest.a
    libgtest_main.a
)