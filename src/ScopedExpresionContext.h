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
        void setVariableUpdater(VariableUpdater* pVariableUpdater, bool deleteIt = false);
        void fillVariable(const char* name, Variable* resultVariable);

        static ScopedExpresionContext* current();
    };

    template <class Ft>
    void setVariableUpdater(Ft&& fx) {
        auto context = ScopedExpresionContext::current();
        if(context == nullptr) {
            throw std::runtime_error("There is no expression context in the current scope");
        }
        context->setVariableUpdater(new FunctionalVariableUpdater<Ft>(std::move(fx)), true);
    }
}