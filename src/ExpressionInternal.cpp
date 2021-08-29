#include "ExpressionInternal.h"

namespace xpression
{
    RaiiScopeExecutor::RaiiScopeExecutor(RaiiScopeFunction&& fx) : _fx(fx) {}
    RaiiScopeExecutor::~RaiiScopeExecutor() {
        _fx();
    }
} // namespace xpression
