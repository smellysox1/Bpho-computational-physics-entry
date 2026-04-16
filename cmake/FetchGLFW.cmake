add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw)

include_directories(${CMAKE_SOURCE_DIR}/vendor/glfw/include)

list(APPEND LibrariesToAppend "glfw")