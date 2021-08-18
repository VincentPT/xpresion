#pragma once

#include "ExpressionCpp.h"

namespace dynamicexp {
    class ExpressionContext;

    class EXPRESSIONCPP_API ScopedExpresionContext {
        ExpressionContext* _pExpressionContext;
        ExpressionContext* _pPreviousExpressionContext;
    public:
        ScopedExpresionContext();
        ~ScopedExpresionContext();
    };
}