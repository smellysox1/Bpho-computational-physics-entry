cmake_minimum_required(VERSION 3.13)
project(bpho_computational_physics_challenge_entry)

set(TASK brown-ian-motion)
set(SEED_FINDER_MODE OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${TASK})

set(SFML_SHARED_LIRARIES FALSE)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_BUILD_WINDOW TRUE)
set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

if (${TASK} STREQUAL randomWalk)
    if (SEED_FINDER_MODE)
        file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/randomWalk/seed_finder/*.cpp)
    else ()
        file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/randomWalk/src/*.cpp)
    endif()

	add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/sfml)
    add_subdirectory(${CMAKE_SOURCE_DIR}/vendor)
	add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/json)

	target_link_libraries(${TASK} PUBLIC
		sfml-system
		sfml-graphics
		sfml-window
		imgui
		imgui-sfml
		nlohmann_json
	)

	add_custom_command(TARGET ${TASK} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/randomWalk/resources/presets.json" ${CMAKE_BINARY_DIR}
        COMMAND_EXPAND_LISTS
	)

elseif(${TASK} STREQUAL brown-ian-motion)
    file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/brown-ian-motion/src/*.cpp)

	add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/sfml)
    add_subdirectory(${CMAKE_SOURCE_DIR}/vendor)

	target_link_libraries(${TASK} PUBLIC
		sfml-system
		sfml-graphics
		sfml-window
		imgui
		imgui-sfml
	)
else ()
    message(FATAL_ERROR "Task ${TASK} does not match any known task.")
endif ()

target_sources(${TASK} PUBLIC ${SRC_FILES})