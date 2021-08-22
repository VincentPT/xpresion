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
        ScopedExpresionContext();
        ~ScopedExpresionContext();

        void setCustomScript(const wchar_t* customScript);
        void addVariable(Variable* pVariable);
        VariableUpdater* getVariableUpdater();
        void setVariableUpdater(VariableUpdater* pVariableUpdater);
    };
}