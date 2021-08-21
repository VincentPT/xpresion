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
        CompiledExpressionRef compileExpressionInProgramContext(const std::wstring& expstr);
        Program* compileProgram(const wchar_t* codeStart, const wchar_t* codeEnd);
        CLamdaProg* detachProgram(Program* program);
        const TypeManagerRef& getTypeManager() const;
		void setPreprocessor(const PreprocessorRef& preprocessor);
		const PreprocessorRef getPreprocessor() const;
        void getLastCompliedPosition(int& line, int& column);
        ScriptCompilerRef& getCompiler();
    };
}