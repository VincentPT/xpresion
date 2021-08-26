#include "ScopedExpresionContext.h"
#include "ExpressionContext.h"

namespace xpression {
    ScopedExpresionContext::ScopedExpresionContext() {
        _pPreviousExpressionContext = ExpressionContext::getCurrentContext();
        _pExpressionContext = new ExpressionContext();

        UserData userData = {0};
        userData.data = this;
        userData.dt = UserDataType::ScopedContext;

        _pExpressionContext->setUserData(userData);
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

    void ScopedExpresionContext::addVariable(Variable* pVariable) {
        _pExpressionContext->addVariable(pVariable);
    }

    void ScopedExpresionContext::removeVariable(Variable* pVariable) {
        _pExpressionContext->removeVariable(pVariable);
    }
    
    VariableUpdater* ScopedExpresionContext::getVariableUpdater() {
        return _pExpressionContext->getVariableUpdater();
    }
    
    void ScopedExpresionContext::setVariableUpdater(VariableUpdater* pVariableUpdater) {
        _pExpressionContext->setVariableUpdater(pVariableUpdater);
    }

    ScopedExpresionContext* ScopedExpresionContext::current() {
        auto expressionContext = ExpressionContext::getCurrentContext();
        auto& userData = expressionContext->getUserData();
        if(userData.dt == UserDataType::ScopedContext) {
            return (ScopedExpresionContext*)userData.data;
        }
        return nullptr;
    }
}