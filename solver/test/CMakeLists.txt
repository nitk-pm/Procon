
add_executable(loader_test loader_test.cpp)
target_link_libraries(loader_test core)

file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt
    DESTINATION ${CMAKE_SOURCE_DIR}/bin
)