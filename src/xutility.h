#pragma once
#include "DataType.h"

namespace ffscript {
    class BasicTypes;
}

namespace xpression {
    DataType dynamicToStatic(const ffscript::BasicTypes& basicType, int dynamicType);
    int staticToDynamic(const ffscript::BasicTypes& basicType, DataType staticType);

    int typeSize(DataType dt);
}