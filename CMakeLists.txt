cmake_minimum_required (VERSION 3.2)
project(dynamic-expression)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# ffscript core project
add_subdirectory(lib/ffscript/ffscript)
# ffscript library project
add_subdirectory(lib/ffscript/ffscriptLibrary)
add_subdirectory(src)
add_subdirectory(Sample)

configure_file(ffscript.cmake.in ${CMAKE_BINARY_DIR}/ffscript.cmake @ONLY)