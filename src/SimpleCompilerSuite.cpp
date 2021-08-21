#include "SimpleCompilerSuite.h"
#include "RawStringLib.h"

#include <exception>

namespace xpression {
    SimpleCompilerSuite::SimpleCompilerSuite() {
        _pCompiler = (ScriptCompilerRef)(new ScriptCompiler());

        // Since we only run a single expression, 128 bytes is enough
        _globalScopeRef = (GlobalScopeRef)(new GlobalScope(128, _pCompiler.get()));

        FunctionRegisterHelper funcLibHelper(_pCompiler.get());
        auto& typeManager = _pCompiler->getTypeManager();

        typeManager->registerBasicTypes(_pCompiler.get());
        typeManager->registerBasicTypeCastFunctions(_pCompiler.get(), funcLibHelper);
        typeManager->registerConstants(_pCompiler.get());

        importBasicfunction(funcLibHelper);
		includeRawStringToCompiler(_pCompiler.get());

        _pCompiler->beginUserLib();
    }


    SimpleCompilerSuite::~SimpleCompilerSuite() {
    }

    CompiledExpressionRef SimpleCompilerSuite::compileExpression(const std::wstring& expstr) {
		ExpressionParser parser(_pCompiler.get());
		_pCompiler->pushScope(_globalScopeRef.get());

		_globalScopeRef->setBeginCompileChar(expstr.c_str());

		list<ExpUnitRef> units;
		EExpressionResult eResult = parser.tokenize(expstr, units);
		_globalScopeRef->setErrorCompilerChar(parser.getLastCompileChar());

		if (eResult != EE_SUCCESS) {
			throw std::runtime_error (_pCompiler->getLastError());
		}

		list<ExpressionRef> expList;
		bool res = parser.compile(units, expList);
		if (res == false) {
            throw std::runtime_error (_pCompiler->getLastError());
        }

		Expression* expressionPtr = expList.front().get();
		eResult = parser.link(expressionPtr);
		if (eResult != EE_SUCCESS) {
            throw std::runtime_error (_pCompiler->getLastError());
        }

		//all variable in the scope will be place at right offset by bellow command
		//if this function is not execute before extract the code then all variable
		//will be placed at offset 0
		_globalScopeRef->updateVariableOffset();

		ExpUnitExecutor* pExcutor = new ExpUnitExecutor(_globalScopeRef.get());
		pExcutor->extractCode(_pCompiler.get(), expressionPtr);


		return std::make_shared<CompiledExpression>(expList.front(), ExpUnitExecutorRef(pExcutor));
	}

    const TypeManagerRef& SimpleCompilerSuite::getTypeManager() const {
		return _pCompiler->getTypeManager();
	}
}