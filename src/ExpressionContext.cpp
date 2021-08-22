#include "ExpressionContext.h"
#include "SimpleCompilerSuite.h"
#include "xutility.h"

#include <string.h>
#include <DefaultPreprocessor.h>
#include <CLamdaProg.h>
#include <Program.h>
#include <string>
#include "VariableManager.h"

namespace xpression {
#if _WIN32 || _WIN64
	__declspec(thread) ExpressionContext* _threadExpressionContext = nullptr;
// Check GCC
#elif __GNUC__
	__thread ExpressionContext* _threadExpressionContext = nullptr;
#endif

    ExpressionContext::ExpressionContext() :
        _pRawProgram(nullptr), _pCustomScript(nullptr),
        _pVariableManager(nullptr) {
        _pCompilerSuite = new SimpleCompilerSuite();
        _pCompilerSuite->setPreprocessor(std::make_shared<DefaultPreprocessor>());
        _userData.data = nullptr;
        _userData.dt = UserDataType::NotUsed;
        _pVariableManager = new VariableManager(_pCompilerSuite->getGlobalScope()->getContext());
    }

    ExpressionContext::~ExpressionContext() {
        if(_pCompilerSuite) {
            delete _pCompilerSuite;
        }
        if(_pVariableManager) {
            delete _pVariableManager;
        }
        stopEvaluating();
    }

    SimpleCompilerSuite* ExpressionContext::getCompilerSuite() const {
        return _pCompilerSuite;
    }

    ExpressionContext* ExpressionContext::getCurrentContext() {
        return _threadExpressionContext;
    }

    void ExpressionContext::setCurrentContext(ExpressionContext* pContext) {
        _threadExpressionContext = pContext;
    }

    void ExpressionContext::setCustomScript(const wchar_t* customScript) {
        if(_pRawProgram || _pCustomScript) {
             throw std::runtime_error("custom script is already available");
        }
        auto len = wcslen(customScript);
        _pRawProgram = _pCompilerSuite->compileProgram(customScript, customScript + len);
        _pCustomScript = nullptr;

        // check if the compiling process is failed...
        if (_pRawProgram == nullptr) {
            // ...then get error information
            // then shows to user
            int line, column;
            _pCompilerSuite->getLastCompliedPosition(line, column);

            string errorMsg("error at line = ");
            errorMsg.append(std::to_string(line + 1));
            errorMsg.append(", column = ");
            errorMsg.append(std::to_string(column + 1));
            errorMsg.append(" msg: ");
            errorMsg.append(_pCompilerSuite->getCompiler()->getLastError());
            
            throw std::runtime_error(errorMsg);
        }
    }

    void ExpressionContext::startEvaluating() {
        if(_pVariableManager) {
            _pCompilerSuite->getGlobalScope()->updateVariableOffset();
            _pVariableManager->requestUpdateVariables();
            _pVariableManager->checkVariablesFullFilled();
        }

        // no need to run global code
        if(_pRawProgram == nullptr) return;

        // evaluating run global code
        if(_pCustomScript) return;        
        
        _pCustomScript = _pCompilerSuite->detachProgram(_pRawProgram);
        _pRawProgram = nullptr;
        _pCustomScript->runGlobalCode();

#ifdef MEMORY_SAVING
        // compiler suite is no longer necessary
        delete _pCompilerSuite;
        _pCompilerSuite = nullptr;
#endif
    }

    void ExpressionContext::stopEvaluating() {
        if(_pRawProgram) {
            delete _pRawProgram;
        }
        if(_pCustomScript) {
            _pCustomScript->cleanupGlobalMemory();
            delete _pCustomScript;
        }
    }

    void ExpressionContext::addVariable(xpression::Variable* pVariable) {
        if(pVariable->name == nullptr || pVariable->name[0] == 0) {
            throw std::runtime_error("Variable name cannot be empty");
        }
        if(pVariable->type == DataType::Unknown) {
            throw std::runtime_error("type of variable '" + std::string(pVariable->name) + "' is unknown");
        }
        
        auto globalScope = _pCompilerSuite->getGlobalScope();

        // convert static type to lambda script type
        auto& typeManager = _pCompilerSuite->getTypeManager();
        auto& basicType = typeManager->getBasicTypes();
        int iType = staticToDynamic(basicType, pVariable->type);
        if(DATA_TYPE_UNKNOWN == iType) {
            throw std::runtime_error("type of variable '" + std::string(pVariable->name) + "' is not supported");
        }

        // regist variable with specified name
        auto pScriptVariable = globalScope->registVariable(pVariable->name);
        if(pScriptVariable == nullptr) {
            throw std::runtime_error("Variable '" + std::string(pVariable->name) + "' is already exist");
        }

        // set variable type
        ScriptType type(iType, typeManager->getType(iType));
        pScriptVariable->setDataType(type);

        _pVariableManager->addVariable(pVariable);
        _pVariableManager->addRequestUpdateVariable(pScriptVariable, pVariable->dataPtr == nullptr);
    }

    VariableUpdater* ExpressionContext::getVariableUpdater() {
        if(_pVariableManager == nullptr) {
            return nullptr;
        }

        return _pVariableManager->getVariableUpdater();
    }

    void ExpressionContext::setVariableUpdater(VariableUpdater* pVariableUpdater) {
        _pVariableManager->setVariableUdater(pVariableUpdater);
    }

    void ExpressionContext::setUserData(const UserData& userData) {
        _userData = userData;
    }

    UserData& ExpressionContext::getUserData() {
        return _userData;
    }
}