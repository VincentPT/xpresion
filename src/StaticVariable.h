#pragma once
#include "DataType.h"
#include <string>

namespace xpression {
#pragma pack(push, 4)
    // transition object to set variable for an expression context
    struct Variable {
        const char* name;
        void* dataPtr;
        int dataSize;
        DataType type;
    };
#pragma pack(pop)
}