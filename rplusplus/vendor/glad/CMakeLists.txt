cmake_minimum_required(VERSION 3.13)
project(glad)

add_library(glad)

target_sources(${PROJECT_NAME} PUBLIC src/glad.c)

target_include_directories(${PROJECT_NAME} PUBLIC include)