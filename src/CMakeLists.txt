cmake_minimum_required(VERSION 3.2)
project(xpression C CXX)

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
	xutility.cpp
	xutility.h
	StaticVariable.h
	VariableManager.h
	VariableManager.cpp
	VariableUpdater.h
	VariableUpdater.cpp
	xpression.h
	xpression.cpp
	ImmediateScope.h
	ImmediateScope.cpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCE_FILES})
add_definitions(-DEXPORT_EXPRESSIONCPP)
target_link_libraries(${PROJECT_NAME} PRIVATE ffscriptLibrary)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})