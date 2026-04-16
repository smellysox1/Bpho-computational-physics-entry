project(imgui-sfml)

add_library(imgui-sfml)

target_sources(imgui-sfml PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui-sfml/imgui-SFML.cpp)

target_include_directories(imgui-sfml PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui-sfml)

target_link_libraries(imgui-sfml PUBLIC
    imgui
    sfml-system
    sfml-graphics
    sfml-window
)

list(APPEND LibrariesToAppend "imgui-sfml")