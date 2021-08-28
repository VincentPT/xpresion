cmake_minimum_required(VERSION 3.2)
project(xpressiontester C CXX)

if (WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../lib/ffscript/gtest/conan/conanbuildinfo_multi.cmake)
else (WIN32)
	include(${CMAKE_CURRENT_SOURCE_DIR}/../lib/ffscript/gtest/conan/conanbuildinfo.cmake)
endif (WIN32)

conan_basic_setup()

SET (PROJECT_SOURCE_FILES
	PrimaryTests.cpp
    CustomScriptTests.cpp
    GlobalContextVariableTests.cpp
    AutoVariableTests.cpp
    VariableWithCustomScriptTest.cpp
    LocalContextVariableTests.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCE_FILES})

conan_target_link_libraries(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} xpression)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:xpression>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)