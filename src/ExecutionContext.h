#pragma once
#include "ExpUnitExecutor.h"
#include "ExpressionContext.h"
#include "CompiledExpression.h"

using namespace ffscript;

namespace dynamicexp {
    class ExcutionContext {
        ExpressionContext* _compilationContext;
        ExpUnitExecutorRef _compiledResult;
    public:
        ExcutionContext(ExpressionContext* compilationContext, const ExpUnitExecutorRef& compiledResult);
        ~ExcutionContext();

        const ExpUnitExecutorRef& getCompiledResult() const;
        const ExpressionContext* getCompilationContext() const;
    };
}