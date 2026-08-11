cmake_minimum_required(VERSION 3.13)
project(bpho_computational_physics_challenge_entry)

set(TASK brownian_motion)
set(SEED_FINDER_MODE OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${TASK})

include("${CMAKE_SOURCE_DIR}/cmake/LinkLibraries.cmake")

if ("${TASK}" STREQUAL "random_walk")
    if (SEED_FINDER_MODE)
        file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/random_walk/seed_finder/*.cpp")
    else ()
        file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/random_walk/src/*.cpp")

		LinkLibraries(
			SFML
			ImGuiSFML
			JSON
		)

		include("${CMAKE_SOURCE_DIR}/cmake/CopyResources.cmake")
		CopyResources("${CMAKE_SOURCE_DIR}/random_walk/resources/presets.json")
    endif()
elseif ("${TASK}" STREQUAL "brownian_motion")
	file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/${TASK}/src/*.cpp")

	LinkLibraries(
		SFML
		ImGuiSFML
	)
elseif ("${TASK}" STREQUAL "photoelectric_effect")
    file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/photoelectric_effect/src/*.cpp")

	LinkLibraries(
		SFML
		ImGuiSFML
	)
elseif ("${TASK}" STREQUAL "electron_diffraction")
    file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/electron_diffraction/src/*.cpp")

	LinkLibraries(
		GLFW
		Glad
		ImGuiGLFW
	)
elseif ("${TASK}" STREQUAL "black_body_radiation_spectrum")
    file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/black_body_radiation_spectrum/src/plancks_law.cpp")

	add_executable("heat_capacity" "${CMAKE_SOURCE_DIR}/black_body_radiation_spectrum/src/heat_capacity.cpp")

	LinkLibraries(
		R++
	)

	target_link_libraries("heat_capacity" rplusplus)
elseif ("${TASK}" STREQUAL "quantum_cryptography")
    file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/quantum_cryptography/src/*.cpp")

	LinkLibraries(
		GLFW
		Glad
		ImGuiGLFW
	)

	target_include_directories(${TASK} PRIVATE "${CMAKE_SOURCE_DIR}/vendor/stb")
elseif (${TASK} STREQUAL "hydrogenic_orbitals")
    file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/hydrogenic_orbitals/src/*.cpp")

	LinkLibraries(
		GLFW
		Glad
		ImGuiGLFW
		GLM
	)
else ()
    message(FATAL_ERROR "Task ${TASK} does not match any known task.")
endif ()

target_sources("${TASK}" PUBLIC "${SRC_FILES}")