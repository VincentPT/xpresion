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
}