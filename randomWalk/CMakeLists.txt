cmake_minimum_required(VERSION 3.13)
project(randomwalk)

# enable or disable seed finder mode
set(SEED_FINDER_MODE ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (SEED_FINDER_MODE)
	file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/seed_finder/*.cpp)
	add_executable(seed_finder ${SRC_FILES})
else ()
	file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
	add_executable(${PROJECT_NAME} ${SRC_FILES})

	set(SFML_SHARED_LIRARIES FALSE)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_BUILD_WINDOW TRUE)
	set(SFML_BUILD_GRAPHICS TRUE)
	set(SFML_BUILD_AUDIO FALSE)
	set(SFML_BUILD_NETWORK FALSE)

	add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/SFML)

	add_library(imgui)

	target_sources(imgui PUBLIC
		${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
		${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
		${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
		${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
	)

	target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui)

	add_library(imgui-sfml)

	target_sources(imgui-sfml PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui-sfml/imgui-SFML.cpp)

	target_include_directories(imgui-sfml PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui-sfml)

	target_link_libraries(imgui-sfml PUBLIC
		imgui
		sfml-system
		sfml-graphics
		sfml-window
	)

	target_link_libraries(${PROJECT_NAME} PUBLIC
		sfml-system
		sfml-graphics
		sfml-window
		imgui
		imgui-sfml
	)

	add_custom_command(
	TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_SOURCE_DIR}/resources/arial.ttf" ${CMAKE_BINARY_DIR}
	COMMAND_EXPAND_LISTS
	)

endif()