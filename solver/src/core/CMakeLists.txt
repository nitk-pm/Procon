
file(GLOB CORE_FILE *.h *.cpp)
include_directories(../)

add_library(core ${CORE_FILE})