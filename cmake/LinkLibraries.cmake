macro(LinkLibraries)
    foreach(library IN ITEMS ${ARGN})
        message(STATUS "Linking ${library}")
        include("${CMAKE_SOURCE_DIR}/cmake/Fetch${library}.cmake")
    endforeach()

    target_link_libraries(${TASK} PUBLIC ${LibrariesToAppend})
endmacro()