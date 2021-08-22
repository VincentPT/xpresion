#pragma once
#include "StaticVariable.h"
#include "xpression.h"

namespace xpression {
    class EXPRESSIONCPP_API VariableUpdater {
    public:
        virtual bool onRequestUpdate(Variable* pVariable) = 0;
    };
}