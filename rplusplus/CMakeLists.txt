cmake_minimum_required(VERSION 3.10)
project(rplusplus)

add_library(rplusplus)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE "${SRC_FILES}")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include)

add_library(imgui)

target_sources(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    glad
    imgui
)