set(SFML_SHARED_LIRARIES FALSE)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_BUILD_WINDOW TRUE)
set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/sfml)

list(APPEND LibrariesToAppend "sfml-system;sfml-graphics;sfml-window")