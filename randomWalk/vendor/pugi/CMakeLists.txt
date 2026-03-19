cmake_minimum_required(VERSION 3.10)
project(pugi)

add_library(pugi)

target_sources(pugi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/pugixml.cpp)

target_include_directories(pugi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})