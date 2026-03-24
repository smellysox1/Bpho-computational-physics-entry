cmake_minimum_required(VERSION 3.13)
project(imgui)

add_library(imgui)

target_sources(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui)

project(imgui-sfml)

add_library(imgui-sfml)

target_sources(imgui-sfml PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui-sfml/imgui-SFML.cpp)

target_include_directories(imgui-sfml PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui-sfml)

target_link_libraries(imgui-sfml PUBLIC
    imgui
    sfml-system
    sfml-graphics
    sfml-window
)