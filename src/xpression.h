#pragma once

#ifdef _WIN32
#ifdef EXPORT_EXPRESSIONCPP
#define EXPRESSIONCPP_API __declspec(dllexport)
#else
#define EXPRESSIONCPP_API __declspec(dllimport)
#endif // EXPORT_EXPRESSIONCPP
#else
#define GAME_CONTROLLER_API
#endif // _WIN32

#include "DataType.h"
#include "ExpressionCpp.h"
#include "ScopedExpresionContext.h"