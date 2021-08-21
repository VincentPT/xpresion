#pragma once

#include "ExpressionCpp.h"

namespace xpression {
    class ExpressionContext;

    class EXPRESSIONCPP_API ScopedExpresionContext {
        ExpressionContext* _pExpressionContext;
        ExpressionContext* _pPreviousExpressionContext;
    public:
        ScopedExpresionContext();
        ~ScopedExpresionContext();

        void setCustomScript(const wchar_t* customScript);
    };
}