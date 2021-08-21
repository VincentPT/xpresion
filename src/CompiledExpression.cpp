#include "CompiledExpression.h"

namespace xpression {
    CompiledExpression::CompiledExpression(const ExecutableUnitRef& internalExpressionRoot, const ExpUnitExecutorRef& executor) :
    _internalExpressionRoot(internalExpressionRoot), _expresionExecutor(executor) {}
    
    CompiledExpression::~CompiledExpression() {}

    const ExecutableUnitRef& CompiledExpression::getInternalExpresionRoot() const {
        return _internalExpressionRoot;
    }
    const ExpUnitExecutorRef& CompiledExpression::getExecutor() const {
        return _expresionExecutor;
    }
}