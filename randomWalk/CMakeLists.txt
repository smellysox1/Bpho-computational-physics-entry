cmake_minimum_required(VERSION 3.13)
project(wechatoncemore)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMP0079 NEW)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor)
