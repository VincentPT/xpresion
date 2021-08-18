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
#include "CompiledExpression.h"
#include <string>

using namespace ffscript;

namespace xpression {
    
    class SimpleCompilerSuite {
        ScriptCompilerRef _pCompiler;
        GlobalScopeRef _globalScopeRef;
        PreprocessorRef _preprocessor;
    public:
        SimpleCompilerSuite();
        ~SimpleCompilerSuite();

        CompiledExpressionRef compileExpression(const std::wstring& expstr);
        const TypeManagerRef& getTypeManager() const;
    };
}