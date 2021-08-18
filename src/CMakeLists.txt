cmake_minimum_required(VERSION 3.2)
project(ExpressionCpp C CXX)

SET (PROJECT_SOURCE_FILES
	ExpressionCpp.cpp
	ExpressionCpp.h
	SimpleCompilerSuite.h
	SimpleCompilerSuite.cpp
	CompiledExpression.h
	CompiledExpression.cpp
	ExpressionContext.h
	ExpressionContext.cpp
	ScopedExpresionContext.h
	ScopedExpresionContext.cpp
	Utility.cpp
	Utility.h
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCE_FILES})
add_definitions(-DEXPORT_EXPRESSIONCPP)
target_link_libraries(${PROJECT_NAME} ffscriptLibrary)