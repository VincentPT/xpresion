#include "ExpressionContext.h"
#include "SimpleCompilerSuite.h"
#include "xutility.h"

#include <string.h>
#include <DefaultPreprocessor.h>
#include <CLamdaProg.h>
#include <Program.h>
#include <string>
#include <list>
#include "VariableManager.h"

namespace xpression {
#if _WIN32 || _WIN64
	__declspec(thread) ExpressionContext* _threadExpressionContext = nullptr;
// Check GCC
#elif __GNUC__
	__thread ExpressionContext* _threadExpressionContext = nullptr;
#endif

    class ExpressionEventManager {
        std::list<ExpressionEventHandler*> _handlers;
    public:
        void addHandler(ExpressionEventHandler* handler) {
            _handlers.push_back(handler);
        }

        void removeHandler(ExpressionEventHandler* handler) {
            _handlers.remove(handler);
        }

        void notifyUpdate() {
            for(auto hander : _handlers) {
                hander->onRequestUpdate();
            }
        }
    };

    ExpressionContext::ExpressionContext() :
        _pRawProgram(nullptr), _allocatedDataSize(0), _allocatedScopeSize(0),
        _pVariableManager(nullptr), _needUpdateVariable(false),
        _needRunGlobalCode(false), _needRegenerateCode(false) {
        _pCompilerSuite = new SimpleCompilerSuite();
        _pCompilerSuite->setPreprocessor(std::make_shared<DefaultPreprocessor>());
        _userData.data = nullptr;
        _userData.dt = UserDataType::NotUsed;
        _pVariableManager = new VariableManager(_pCompilerSuite->getGlobalScope()->getContext());
        _eventManager = new ExpressionEventManager();
    }

    ExpressionContext::~ExpressionContext() {
        if(_pRawProgram) {
            delete _pRawProgram;
        }
        if(_pCompilerSuite) {
            delete _pCompilerSuite;
        }
        if(_pVariableManager) {
            delete _pVariableManager;
        }
        if(_eventManager) {
            delete _eventManager;
        }
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
        if(_pRawProgram) {
             throw std::runtime_error("custom script is already available");
        }
        auto len = wcslen(customScript);
        _pRawProgram = _pCompilerSuite->compileProgram(customScript, customScript + len);

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
        _needRunGlobalCode = true;
    }

    void ExpressionContext::startEvaluating() {
        if(_pVariableManager && _needUpdateVariable) {
            _pVariableManager->requestUpdateVariables();
            _pVariableManager->checkVariablesFullFilled();
            // all registered variables are updated now
            _needUpdateVariable = false;
        }

        auto globalScope = _pCompilerSuite->getGlobalScope();
        if(_needRegenerateCode && _pRawProgram) {
            _pRawProgram->resetExcutors();
            _pRawProgram->resetFunctionMap();
            if(!globalScope->extractCode(_pRawProgram)) {
                throw std::runtime_error("Regenerate code is failed");
            }
            _needRunGlobalCode = true;
        }

        // check if need to run global code
        if(!_needRunGlobalCode) return;
        
        auto context = globalScope->getContext();

        if(_allocatedDataSize || _allocatedScopeSize) {
            context->scopeUnallocate(_allocatedDataSize, _allocatedScopeSize);
        }

        _allocatedDataSize = globalScope->getDataSize();
        _allocatedScopeSize = globalScope->getScopeSize(); // space need to run the code

        context->pushContext(globalScope->getConstructorCommandCount());
		context->scopeAllocate(_allocatedDataSize, _allocatedScopeSize);

		context->run();

        context->runDestructorCommands();
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
        // the variable now is register success, then it need evaluate before expression evaluate
        _needUpdateVariable = true;

        // request existing expression in current scope update when new variable is added to the scope
        _eventManager->notifyUpdate();

        // update the last added variable' offset
        globalScope->updateLastVariableOffset();
        // need re-generate code after add new variable
        _needRegenerateCode = true;
    }

    void ExpressionContext::removeVariable(Variable* pVariable) {
        _pVariableManager->removeVariable(pVariable);
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

    void ExpressionContext::addExpressionEventHandler(ExpressionEventHandler* handler) {
        _eventManager->addHandler(handler);
    }

    void ExpressionContext::removeExpressionEventHandler(ExpressionEventHandler* handler) {
        _eventManager->removeHandler(handler);
    }

    void ExpressionContext::fillVariable(const char* name, Variable* resultVariable) {
        auto globalScope = _pCompilerSuite->getGlobalScope();
        auto& compiler = _pCompilerSuite->getCompiler();
        auto scriptVarible = globalScope->findVariable(name);
        if(scriptVarible == nullptr) {
            throw std::runtime_error("variable '" + std::string(name) + "'not found");
        }

        // need run global code before fill variable 
        startEvaluating();
        auto variableContext = globalScope->getContext();

        auto& typeManager = _pCompilerSuite->getTypeManager();
        auto& basicType = typeManager->getBasicTypes();
        resultVariable->type = dynamicToStatic(basicType, scriptVarible->getDataType().iType());
        resultVariable->dataSize = compiler->getTypeSize(scriptVarible->getDataType());

        void* variableDataAddressDst =
                    variableContext->getAbsoluteAddress(variableContext->getCurrentOffset() + scriptVarible->getOffset());

        resultVariable->dataPtr = variableDataAddressDst;
    }
}