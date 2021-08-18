#include "ExecutionContext.h"

namespace dynamicexp {
    ExcutionContext::ExcutionContext(ExpressionContext* compilationContext, const ExpUnitExecutorRef& compiledResult) :
    _compilationContext(compilationContext),
    _compiledResult(compiledResult) {}
    
    ExcutionContext::~ExcutionContext() {}

    const ExpUnitExecutorRef& ExcutionContext::getCompiledResult() const {
        return _compiledResult;
    }

    const ExpressionContext* ExcutionContext::getCompilationContext() const {
        return _compilationContext;
    }
}