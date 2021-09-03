#pragma once
#include "xpressionapi.h"
#include "StaticVariable.h"

namespace xpression {
    class ExpressionContext;
    class VariableUpdater;
    class ExpressionCpp;

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
        void setVariableUpdater(VariableUpdater* pVariableUpdater, bool deleteIt = false);
        void fillVariable(const char* name, Variable* resultVariable);

        static ScopedExpresionContext* current();
    };
}