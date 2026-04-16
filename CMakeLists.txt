cmake_minimum_required(VERSION 3.13)
project(bpho_computational_physics_challenge_entry)

set(TASK electron_diffraction)
set(SEED_FINDER_MODE OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable("${TASK}")

include("${CMAKE_SOURCE_DIR}/cmake/LinkLibraries.cmake")

if ("${TASK}" STREQUAL "randomWalk")
    if (SEED_FINDER_MODE)
        file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/randomWalk/seed_finder/*.cpp")
    else ()
        file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/randomWalk/src/*.cpp")

		LinkLibraries(
			SFML
			ImGui
			ImGuiSFML
			JSON
		)

		include("${CMAKE_SOURCE_DIR}/cmake/CopyResources.cmake")
		CopyResources("${CMAKE_SOURCE_DIR}/randomWalk/resources/presets.json")
    endif()
elseif ("${TASK}" STREQUAL "brown-ian-motion")
	file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/${TASK}/brown-ian-motion/*.cpp")

	LinkLibraries(
		SFML
		ImGui
		ImGuiSFML
	)
elseif ("${TASK}" STREQUAL "photo-electric-jud")
    file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/photo-electric-jud/src/*.cpp")

	LinkLibraries(
		SFML
		ImGui
		ImGuiSFML
		JSON
	)
elseif ("${TASK}" STREQUAL "electron_diffraction")
    file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/electron_diffraction/src/*.cpp")

	LinkLibraries(
		GLFW
		Glad
		ImGui
	)
else ()
    message(FATAL_ERROR "Task ${TASK} does not match any known task.")
endif ()

target_sources("${TASK}" PUBLIC "${SRC_FILES}")