#pragma once
#include "xpression.h"
#include "ExpressionContext.h"
#include <GlobalScope.h>

namespace xpression {
    class ExpressionGlobalScope : public ffscript::GlobalScope {
        std::list<Variable> _variableContainer;
        ExpressionContext* _context;
    public:
        ExpressionGlobalScope(ExpressionContext* context, int stackSize, ffscript::ScriptCompiler* scriptCompiler);
        ~ExpressionGlobalScope();
        ffscript::Variable* findVariable(const std::string&);
    };
}