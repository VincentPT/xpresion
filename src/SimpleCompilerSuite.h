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
#include "ImmediateScope.h"
#include "ExpressionInternal.h"
#include <string>

using namespace ffscript;

namespace xpression {
    
    class SimpleCompilerSuite {
        ScriptCompilerRef _pCompiler;
        GlobalScopeRef _globalScopeRef;
        PreprocessorRef _preprocessor;
    public:
        SimpleCompilerSuite(int stackSize, GlobalScopeCreator* globalScopeCreator);
        ~SimpleCompilerSuite();

        ExpressionRef compileExpression(const std::wstring& expstr, ImmediateScope* pLocalScope = nullptr);
        ExpUnitExecutorRef generateCode(const ExpressionRef& exp);
        //CompiledExpressionRef compileExpressionInProgramContext(const std::wstring& expstr);
        Program* compileProgram(const wchar_t* codeStart, const wchar_t* codeEnd);
        void regenerateProgramCode(Program* Program);
        CLamdaProg* detachProgram(Program* program);
        const TypeManagerRef& getTypeManager() const;
		void setPreprocessor(const PreprocessorRef& preprocessor);
		const PreprocessorRef getPreprocessor() const;
        void getLastCompliedPosition(int& line, int& column);
        ScriptCompilerRef& getCompiler();
        GlobalScope* getGlobalScope();
    };
}