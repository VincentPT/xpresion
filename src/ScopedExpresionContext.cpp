#include "ScopedExpresionContext.h"
#include "ExpressionContext.h"

namespace xpression {
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

    void ScopedExpresionContext::setCustomScript(const wchar_t* customScript) {
        _pExpressionContext->setCustomScript(customScript);
    }
}