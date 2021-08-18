#include "CompiledExpression.h"

namespace dynamicexp {
    CompiledExpression::CompiledExpression(const ExpressionRef& internalExpression, const ExpUnitExecutorRef& executor) :
    _internalExpression(internalExpression), _expresionExecutor(executor) {}
    
    CompiledExpression::~CompiledExpression() {}

    const ExpressionRef& CompiledExpression::getInternalExpresion() const {
        return _internalExpression;
    }
    const ExpUnitExecutorRef& CompiledExpression::getExecutor() const {
        return _expresionExecutor;
    }
}