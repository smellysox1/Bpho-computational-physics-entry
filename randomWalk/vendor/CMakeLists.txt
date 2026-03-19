cmake_minimum_required(VERSION 3.13)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stb)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_BUILD_WINDOW FALSE)
set(SFML_BUILD_GRAPHICS FALSE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui-1.92.5)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SFML)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pugi)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad)

target_link_libraries(${PROJECT_NAME} PUBLIC
	sfml-system
	sfml-network
    sfml-audio
    glad
    glfw
    imgui
    pugi
)