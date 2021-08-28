#pragma once

#include "ExpressionCpp.h"
#include "VariableUpdater.h"

namespace xpression {
    class ExpressionContext;
    class VariableUpdater;

    class EXPRESSIONCPP_API ScopedExpresionContext {
        ExpressionContext* _pExpressionContext;
        ExpressionContext* _pPreviousExpressionContext;
    public:
        ScopedExpresionContext(int stackSize = 128);
        ~ScopedExpresionContext();

        void setCustomScript(const wchar_t* customScript);
        void addVariable(Variable* pVariable);
        void removeVariable(Variable* pVariable);
        VariableUpdater* getVariableUpdater();
        void setVariableUpdater(VariableUpdater* pVariableUpdater);
        void fillVariable(const char* name, Variable* resultVariable);

        static ScopedExpresionContext* current();
    };
}