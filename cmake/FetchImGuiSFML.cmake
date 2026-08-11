add_library(imgui)

target_sources(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/vendor/imgui)

list(APPEND LibrariesToAppend "imgui")

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