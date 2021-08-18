cmake_minimum_required(VERSION 3.2)
project(Sample C CXX)

SET (PROJECT_SOURCE_FILES
	main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ffscriptLibrary)