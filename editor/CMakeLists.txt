cmake_minimum_required(VERSION 3.0)
if (POLICY CMP0020)
cmake_policy(SET CMP0020 OLD)
endif (POLICY CMP0020)

project(editor CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-Wall -O2 -DQT_MESSAGELOGCONTEXT -std=c++14")
set(CMAKE_BUILD_TYPE Release)

#Qtを使用するために必要な処理
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(UI_SOURCE forms/window_layout.ui)
qt5_wrap_ui(UI_HEADER ${UI_SOURCE})
qt5_add_resources(UI_RESOURCES forms/resource.qrc)

file(GLOB SOURCE_FILE src/*/*.cpp src/*/*.h)

add_executable(
    editor
    ${SOURCE_FILE}
    ${UI_SOURCE}
    ${UI_HEADER}
    ${UI_RESOURCES}
)

target_include_directories(editor PRIVATE src)
target_link_libraries(editor Qt5::Widgets Qt5::Core Qt5::Gui)
