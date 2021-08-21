#include "SimpleCompilerSuite.h"
#include "RawStringLib.h"
#include <exception>
#include <memory>

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

		// auto pop scope when the function exit
		std::unique_ptr<ScriptCompiler, std::function<void(ScriptCompiler*)>> autoPopScope(
			_pCompiler.get(),[](ScriptCompiler* compiler){
				compiler->popScope();
			});


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


		return std::make_shared<CompiledExpression>(expressionPtr->getRoot(), ExpUnitExecutorRef(pExcutor));
	}

	CompiledExpressionRef SimpleCompilerSuite::compileExpressionInProgramContext(const std::wstring& expstr) {
		_pCompiler->pushScope(_globalScopeRef.get());

		// auto pop scope when the function exit
		std::unique_ptr<ScriptCompiler, std::function<void(ScriptCompiler*)>> autoPopScope(
			_pCompiler.get(),[](ScriptCompiler* compiler){
				compiler->popScope();
			});
		
		std::wstring expressionScopeScript = L"{" + expstr + L";}";

		ContextScope* contextScope = new ContextScope(_globalScopeRef.get(), nullptr);
		contextScope->parse(expressionScopeScript.c_str(), expressionScopeScript.c_str() + expressionScopeScript.size());
		auto commandIter = contextScope->getFirstCommandUnitRefIter();
		++commandIter;

		ExecutableUnitRef compiledExpresionUnit = std::dynamic_pointer_cast<ExecutableUnit>(*commandIter);

		auto program = _pCompiler->getProgram();

		contextScope->correctAndOptimize(program);
		contextScope->extractCode(program);

		auto endCommand = program->getCommandContainer().end();
		--endCommand;
		--endCommand;

		auto pExcutorRef = std::dynamic_pointer_cast<ExpUnitExecutor>(*endCommand);
		
		return std::make_shared<CompiledExpression>(compiledExpresionUnit, pExcutorRef);
	}

	Program* SimpleCompilerSuite::compileProgram(const wchar_t* codeStart, const wchar_t* codeEnd) {
		_pCompiler->clearUserLib();

		Program* temporaryProgram = new Program();
		_pCompiler->bindProgram(temporaryProgram);

		// delete temporaryProgram in case the script cannot compile
		std::unique_ptr<Program, std::function<void(Program*)>> autoDeletor(
			temporaryProgram,[&temporaryProgram](Program*){
				if(temporaryProgram) delete temporaryProgram;
			});


		if (_preprocessor) {
			auto newCode = _preprocessor->preprocess(codeStart, codeEnd);

			if (_globalScopeRef->parse(newCode->c_str(), newCode->c_str() + newCode->size()) == nullptr) {
				return nullptr;
			}
		}
		else if (_globalScopeRef->parse(codeStart, codeEnd) == nullptr) {
			return nullptr;
		}

		if (_globalScopeRef->correctAndOptimize(temporaryProgram) != 0) {
			return nullptr;
		}

		if (_globalScopeRef->extractCode(temporaryProgram) == false) {
			return nullptr;
		}

		auto program = temporaryProgram;
		// prevent auto delete
		temporaryProgram = nullptr;
		return program;
	}

	CLamdaProg* SimpleCompilerSuite::detachProgram(Program* program) {
		return _globalScopeRef->detachScriptProgram(program);
	}

    const TypeManagerRef& SimpleCompilerSuite::getTypeManager() const {
		return _pCompiler->getTypeManager();
	}

	void SimpleCompilerSuite::setPreprocessor(const PreprocessorRef& preprocessor) {
		_preprocessor = preprocessor;
	}

	const PreprocessorRef SimpleCompilerSuite::getPreprocessor() const {
		return _preprocessor;
	}

	void SimpleCompilerSuite::getLastCompliedPosition(int& line, int& column) {
		if (_preprocessor && _globalScopeRef) {
			auto beginCompileChar = _globalScopeRef->getBeginCompileChar();
			auto lastCompileChar = _globalScopeRef->getErrorCompiledChar();

			_preprocessor->getOriginalPosition((int)(lastCompileChar - beginCompileChar), line, column);
		}
	}

	ScriptCompilerRef& SimpleCompilerSuite::getCompiler() {
		return _pCompiler;
	}
}