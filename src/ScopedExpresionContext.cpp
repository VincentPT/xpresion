#include "ScopedExpresionContext.h"
#include "ExpressionContext.h"

namespace dynamicexp {
    ScopedExpresionContext::ScopedExpresionContext() {
        _pPreviousExpressionContext = ExpressionContext::getCurrentContext();
        _pExpressionContext = new ExpressionContext();
        ExpressionContext::setCurrentContext(_pExpressionContext);
    }
    
    ScopedExpresionContext::~ScopedExpresionContext() {
        ExpressionContext::setCurrentContext(_pPreviousExpressionContext);
        if (_pExpressionContext) {
            delete _pExpressionContext;
        }
    }
}