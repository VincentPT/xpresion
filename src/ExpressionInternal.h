#pragma once
#include <GlobalScope.h>
#include <functional>

namespace xpression {
    class GlobalScopeCreator {
    public:
        virtual ffscript::GlobalScope* create(int stackSize, ffscript::ScriptCompiler* scriptCompiler) = 0;
    };

    typedef std::function<void()> RaiiScopeFunction;
    class RaiiScopeExecutor {
        RaiiScopeFunction _fx;
    public:
        RaiiScopeExecutor(RaiiScopeFunction&& fx);
        ~RaiiScopeExecutor();
    };
}