#pragma once

#include "ScriptCompiler.h"
#include "Program.h"
#include "GlobalScope.h"
#include "StaticContext.h"
#include "BasicFunctionFactory.hpp"
#include "BasicFunction.h"
#include "Context.h"
#include "StaticContext.h"
#include "ContextScope.h"
#include "ScopedCompilingScope.h"
#include "FunctionRegisterHelper.h"
#include "BasicType.h"
#include "ExpUnitExecutor.h"
#include "Preprocessor.h"

using namespace ffscript;

namespace xpression {
    typedef shared_ptr<ExpUnitExecutor> ExpUnitExecutorRef;

    class CompiledExpression {
        ExecutableUnitRef _internalExpressionRoot;
        ExpUnitExecutorRef _expresionExecutor;
    public:
        CompiledExpression(const ExecutableUnitRef& internalExpressionRoot, const ExpUnitExecutorRef& executor);
        ~CompiledExpression();

        const ExecutableUnitRef& getInternalExpresionRoot() const;
        const ExpUnitExecutorRef& getExecutor() const;
    };

    typedef shared_ptr<CompiledExpression> CompiledExpressionRef;
}