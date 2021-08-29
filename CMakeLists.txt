cmake_minimum_required (VERSION 3.2)
project(dynamic-expression)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# ffscript core project
add_subdirectory(lib/ffscript/ffscript)
# ffscript library project
add_subdirectory(lib/ffscript/ffscriptLibrary)
# xpression project
add_subdirectory(src)
# using xpression in a sample project
add_subdirectory(Sample)
# xpression test project
if (NOT SKIP_UNIT_TEST_BUILD) 
    add_subdirectory(test)
endif()

configure_file(ffscript.cmake.in ${CMAKE_BINARY_DIR}/ffscript.cmake @ONLY)