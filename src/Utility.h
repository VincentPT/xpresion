#pragma once
#include "DataType.h"
#include "BasicType.h"

#include <string>

using namespace ffscript;

namespace dynamicexp {
    DataType dynamicToStatic(const BasicTypes& basicType, int dynamicType);
    int staticToDynamic(const BasicTypes& basicType, DataType staticType);
    DataType cppToStatic(bool);
    DataType cppToStatic(int);
    DataType cppToStatic(long long);
    DataType cppToStatic(double);
    DataType cppToStatic(float);
    DataType cppToStatic(char);
    DataType cppToStatic(wchar_t);
    DataType cppToStatic(const std::string&);
    DataType cppToStatic(const std::wstring&);
}