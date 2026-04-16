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

list(APPEND LibrariesToAppend "imgui")