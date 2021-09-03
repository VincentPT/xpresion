#include "ExpressionCpp.h"
#include "DataType.h"
#include "SimpleCompilerSuite.h"
#include "ExpressionContext.h"
#include "xutility.h"
#include "ImmediateScope.h"
#include "VariableManager.h"
#include "xpression.hpp"
#include <Utils.h>

#include <exception>
#include <string.h>
#include <memory>

namespace xpression {

    const char* getTypeName(DataType dt) {
        switch (dt)
        {
        case DataType::AsciiString :
            return "AsciiString";
        case DataType::Boolean :
            return "Boolean";
        case DataType::Char :
            return "Char";
        case DataType::Double :
            return "Double";
        case DataType::Float :
            return "Float";
        case DataType::Integer :
            return "Integer";
        case DataType::Long :
            return "Long";
        case DataType::String :
            return "String";
        case DataType::UnicodeString :
            return "UnicodeString";
        case DataType::Void :
            return "Void";
        case DataType::Wchar :
            return "Wchar";
        default:
            break;
        }
        return "Unknown";
    }

    class ExpressionLocalScope : public ImmediateScope {
        InternalExpressionCpp* _context;
        std::list<Variable> _variableContainer;
    public:
        ExpressionLocalScope(InternalExpressionCpp* pInternalExpression, ffscript::ScriptCompiler* scriptCompiler) :
            ImmediateScope(scriptCompiler), _context(pInternalExpression) {}
        ~ExpressionLocalScope() {}

        ffscript::Variable* findVariable(const std::string& name);
    };
    
    class InternalExpressionCpp : public ExpressionEventHandler {
        friend class ExpressionCpp;
        ExpressionContext* _compilationContext;
        VariableManager* _pVariableManager;
        ImmediateScope* _expressionScope;
        ExpressionRef _compiledResult;
        ExpUnitExecutorRef _codeExecutor;
        wstring _expresionStr;
        DataType _resultType;
        typedef std::unique_ptr<void, std::function<void(void*)>> ResultDeletor;
        ResultDeletor _manualDeletedResult;
        bool _evaluated;
        bool _needUpdateVariable;
    public:
        InternalExpressionCpp(): _evaluated(false),
            _resultType(DataType::Unknown), _expressionScope(nullptr),
            _pVariableManager(nullptr), _needUpdateVariable(false)  {
            _compilationContext = ExpressionContext::getCurrentContext();
            if( _compilationContext == nullptr) {
                throw std::runtime_error("No instance of ExpressionContext for current thread found");
            }
            _compilationContext->addExpressionEventHandler(this);

            auto compilerSuite = _compilationContext->getCompilerSuite();
            _expressionScope = new ExpressionLocalScope(this, compilerSuite->getCompiler().get());
            _expressionScope->setParent(compilerSuite->getGlobalScope());

            _pVariableManager = new VariableManager(compilerSuite->getGlobalScope()->getContext());
        }
        ~InternalExpressionCpp() {
            if(_expressionScope) {
                delete _expressionScope;
            }
            if(_pVariableManager) {
                delete _pVariableManager;
            }
            _compilationContext->removeExpressionEventHandler(this);
        }

        void compile() {
            auto compilerSuite = _compilationContext->getCompilerSuite();
            if(compilerSuite == nullptr) {
                throw std::runtime_error("Compiler is no longer exist in current context due to memory saving process is enable");
            }

            auto& typeManager = compilerSuite->getTypeManager();
            auto& basicTypes = typeManager->getBasicTypes();

            auto program = compilerSuite->getCompiler()->getProgram();

             _codeExecutor.reset();
            _compiledResult = compilerSuite->compileExpression(_expresionStr, _expressionScope);

            auto dynamicReturnType = _compiledResult->getRoot()->getReturnType().iType();
            _resultType = dynamicToStatic(basicTypes, dynamicReturnType);
        }

        void generateCode() {
            auto compilerSuite = _compilationContext->getCompilerSuite();
            if(compilerSuite == nullptr) {
                throw std::runtime_error("Compiler is no longer exist in current context due to memory saving process is enable");
            }
            auto globalScope = compilerSuite->getGlobalScope();
            auto oldBaseOffset = globalScope->getBaseOffset();
            globalScope->setBaseOffset(oldBaseOffset + _expressionScope->getDataSize());

            // auto restore base offset
            RaiiScopeExecutor autoRestoreBaseOffset(
                [globalScope, oldBaseOffset](){
                     globalScope->setBaseOffset(oldBaseOffset);
                });

            _codeExecutor = compilerSuite->generateCode(_compiledResult);
        }

        void evaluate() {
            if(!_compiledResult) compile();

            _compilationContext->startEvaluating();
            Context* context = Context::getCurrent();

            // copy data from static variable to script variable
            if(_pVariableManager && _needUpdateVariable) {
                _expressionScope->setBaseOffset(context->getCurrentScopeSize());
                _expressionScope->updateVariableOffset();

                _pVariableManager->requestUpdateVariables();
                _pVariableManager->checkVariablesFullFilled();
                // all registered variables are updated now
                _needUpdateVariable = false;
            }

            if (!_codeExecutor) generateCode();
            
            // additional variable' space for the expression
            int variableSpace = 0; // => no variable
            // space for expression itself require to run the code
            int requireSpaceToRunCode = _codeExecutor->getLocalSize();

            if(_expressionScope) {
                variableSpace += _expressionScope->getDataSize();
            }

            // allocated buffer enough to run expression
            context->scopeAllocate(variableSpace, requireSpaceToRunCode);
            _codeExecutor->runCode();
            // unallocated the buffer after run expression
            context->scopeUnallocate(variableSpace, requireSpaceToRunCode);

            _evaluated = true;

            if(_resultType == DataType::String) {
                _manualDeletedResult = ResultDeletor(
                    _codeExecutor->getReturnData(),
                    [](void* ptr) {
                        freeRawString(*(RawString*)ptr);
                    });
            }
        }

        template <class T>
        T getResultBasic() {
            if(!_evaluated) {
                throw std::runtime_error("Expresion has not been evaluated");
            }
            auto expectedType = typeFromCpp<T>();
            if(expectedType != _resultType) {
                throw std::runtime_error(std::string("expression result is ") + getTypeName(_resultType) + " not " + getTypeName(expectedType));
            }
            return *(T*)_codeExecutor->getReturnData();
        }

        const wchar_t* getResultString() {
            if(!_evaluated) {
                throw std::runtime_error("Expresion has not been evaluated");
            }
            if(DataType::String == _resultType) {
                return ((RawString*)_codeExecutor->getReturnData())->elms;
            }
            if(DataType::UnicodeString == _resultType) {
                return (*((std::wstring**)_codeExecutor->getReturnData()))->c_str();
            }
            if(DataType::AsciiString == _resultType) {
                auto pRes = *((std::string**)_codeExecutor->getReturnData());
                std::wstring* pTempString = new std::wstring(pRes->begin(), pRes->end());
                _manualDeletedResult = ResultDeletor(
                    pTempString,
                    [](void* ptr) {
                        delete (std::wstring*)ptr;
                    });

                return pTempString->c_str();
            }

            throw std::runtime_error("expression result is not a String");
        }

        DataType getResultType() const {
            if(!_evaluated) {
                throw std::runtime_error("Expresion has not been evaluated");
            }
            return _resultType;
        }

        virtual void onRequestUpdate() {
            _codeExecutor.reset();
        }

        ffscript::Variable* addVariable(Variable* pVariable) {
            auto compilerSuite = _compilationContext->getCompilerSuite();
            auto globalScope = compilerSuite->getGlobalScope();

            if(pVariable->name == nullptr || pVariable->name[0] == 0) {
                throw std::runtime_error("Variable name cannot be empty");
            }
            if(pVariable->type == DataType::Unknown) {
                throw std::runtime_error("type of variable '" + std::string(pVariable->name) + "' is unknown");
            }
            if(pVariable->dataPtr == nullptr) {
                throw std::runtime_error("expression does not support delay variable updating(variable's data is null)");
            }

            // convert static type to lambda script type
            auto& typeManager = compilerSuite->getTypeManager();
            auto& basicType = typeManager->getBasicTypes();
            int iType = staticToDynamic(basicType, pVariable->type);
            if(DATA_TYPE_UNKNOWN == iType) {
                throw std::runtime_error("type of variable '" + std::string(pVariable->name) + "' is not supported");
            }

            // regist variable with specified name
            auto pScriptVariable = _expressionScope->registVariable(pVariable->name);
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

            return pScriptVariable;
        }

        void fillVariable(const char* name, Variable* resultVariable) {
            if(_expressionScope == nullptr) {
                _compilationContext->fillVariable(name, resultVariable);
                return;
            }

            auto pCompilerSuite = _compilationContext->getCompilerSuite();
            auto globalScope = pCompilerSuite->getGlobalScope();
            auto& compiler = pCompilerSuite->getCompiler();
            auto scriptVarible = _expressionScope->findVariable(name);
            if(scriptVarible == nullptr) {
                _compilationContext->fillVariable(name, resultVariable);
                return;
            }

            auto variableContext = globalScope->getContext();

            auto& typeManager = pCompilerSuite->getTypeManager();
            auto& basicType = typeManager->getBasicTypes();
            resultVariable->type = dynamicToStatic(basicType, scriptVarible->getDataType().iType());
            resultVariable->dataSize = compiler->getTypeSize(scriptVarible->getDataType());

            void* variableDataAddressDst =
                        variableContext->getAbsoluteAddress(variableContext->getCurrentOffset() + scriptVarible->getOffset());

            resultVariable->dataPtr = variableDataAddressDst;
        }

        void setVariableUpdater(VariableUpdater* pVariableUpdater, bool deleteIt) {
            _pVariableManager->setVariableUdater(pVariableUpdater, deleteIt);
        }

        VariableUpdater* getVariableUpdater() {
            if(_pVariableManager == nullptr) return nullptr;

            return _pVariableManager->getVariableUpdater();
        }
    };
    
    ffscript::Variable* ExpressionLocalScope::findVariable(const std::string& name) {
        auto pVariable = ImmediateScope::findVariable(name);
        if(pVariable) return pVariable;

        auto variableUpdater =_context->getVariableUpdater();
        if(variableUpdater == nullptr) return nullptr;

        ffscript::Variable* result = nullptr;
        _variableContainer.push_back({0});
        xpression::Variable& variable = _variableContainer.back();

        RaiiScopeExecutor autoCheckResult([&](){
            if(result) {
                // update variable name
                // point variable name to an allocated buffer
                variable.name = result->getName().c_str();
            }
            else {
                // remove the temporary variable due to unsuccessful operation
                _variableContainer.pop_back();
            }
        });
        
        variable.name = name.c_str();
        if(!variableUpdater->onRequestUpdate(&variable)) {
            return nullptr;
        }
        if(variable.type == DataType::Unknown) {
            throw std::runtime_error("the variable '" + name + "' need to specify type when the script is being compiled");
        }
        variable.dataSize = typeSize(variable.type);
        result = _context->addVariable(&variable);
        return result;
    }

    ExpressionCpp::ExpressionCpp(const wchar_t* expStr) {
        _pInternalExpresion = new InternalExpressionCpp();
        _pInternalExpresion->_expresionStr = expStr;
    }

    ExpressionCpp::~ExpressionCpp() {
        delete _pInternalExpresion;
    }

    void ExpressionCpp::compile() {
        _pInternalExpresion->compile();
    }

    void ExpressionCpp::evaluate() {
        _pInternalExpresion->evaluate();
    }

    DataType ExpressionCpp::getResultType() const {
        return _pInternalExpresion->getResultType();
    }

    int ExpressionCpp::getResultInt() {
        return _pInternalExpresion->getResultBasic<int>();
    }

    bool ExpressionCpp::getResultBool() {
        return _pInternalExpresion->getResultBasic<bool>();
    }

    float ExpressionCpp::getResultFloat() {
        return _pInternalExpresion->getResultBasic<float>();
    }

    double ExpressionCpp::getResultDouble() {
        return _pInternalExpresion->getResultBasic<double>();
    }

    long long ExpressionCpp::getResultLong() {
        return _pInternalExpresion->getResultBasic<long long>();
    }

    const wchar_t* ExpressionCpp::getResultString() {
        return _pInternalExpresion->getResultString();
    }

    void ExpressionCpp::addVariable(struct Variable* pVariable) {
        _pInternalExpresion->addVariable(pVariable);
    }

    void ExpressionCpp::fillVariable(const char* name, Variable* resultVariable) {
        _pInternalExpresion->fillVariable(name, resultVariable);
    }

    void ExpressionCpp::setVariableUpdater(VariableUpdater* pVariableUpdater, bool deleteIt) {
        _pInternalExpresion->setVariableUpdater(pVariableUpdater, deleteIt);
    }
}