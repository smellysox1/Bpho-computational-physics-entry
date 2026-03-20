cmake_minimum_required(VERSION 3.13)
project(randomwalk)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set(SFML_SHARED_LIRARIES FALSE)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_BUILD_WINDOW TRUE)
set(SFML_BUILD_GRAPHICS True)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/SFML)

target_link_libraries(${PROJECT_NAME} PUBLIC
	sfml-system
    sfml-graphics
	sfml-window
	sfml-network
    sfml-audio
)

add_custom_command(
TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/resources/arial.ttf" ${CMAKE_BINARY_DIR}
COMMAND_EXPAND_LISTS
)