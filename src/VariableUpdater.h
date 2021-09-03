#pragma once
#include "xpressionapi.h"
#include "StaticVariable.h"

namespace xpression {
    class EXPRESSIONCPP_API VariableUpdater {
    public:
        VariableUpdater();
        virtual ~VariableUpdater();
        virtual bool onRequestUpdate(Variable* pVariable) = 0;
    };
}