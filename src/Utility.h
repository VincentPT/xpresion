#pragma once
#include "DataType.h"
#include "BasicType.h"

#include <string>

using namespace ffscript;

namespace xpression {
    DataType dynamicToStatic(const BasicTypes& basicType, int dynamicType);
    int staticToDynamic(const BasicTypes& basicType, DataType staticType);

    template <typename T>
    DataType typeFromCpp();
}