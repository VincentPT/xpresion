#include "ExpressionCpp.h"
#include "SimpleCompilerSuite.h"
#include "ExpressionContext.h"
#include "xutility.h"
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

    class InternalExpressionCpp : public ExpressionEventHandler {
        friend class ExpressionCpp;
        ExpressionContext* _compilationContext;
        ExpressionRef _compiledResult;
        ExpUnitExecutorRef _codeExecutor;
        wstring _expresionStr;
        bool _evaluated;
        DataType _resultType;
        typedef std::unique_ptr<void, std::function<void(void*)>> ResultDeletor;
        ResultDeletor _manualDeletedResult;
    public:
        InternalExpressionCpp(): _evaluated(false), _resultType(DataType::Unknown)  {
            _compilationContext = ExpressionContext::getCurrentContext();
            if( _compilationContext == nullptr) {
                throw std::runtime_error("No instance of ExpressionContext for current thread found");
            }
            _compilationContext->addExpressionEventHandler(this);
        }
        ~InternalExpressionCpp() {
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

            // auto compilerResult = program ? compilerSuite->compileExpressionInProgramContext(_expresionStr) :
            // compilerSuite->compileExpression(_expresionStr);

             _codeExecutor.reset();
            _compiledResult = compilerSuite->compileExpression(_expresionStr);

            auto dynamicReturnType = _compiledResult->getRoot()->getReturnType().iType();
            _resultType = dynamicToStatic(basicTypes, dynamicReturnType);
        }

        void generateCode() {
            auto compilerSuite = _compilationContext->getCompilerSuite();
            if(compilerSuite == nullptr) {
                throw std::runtime_error("Compiler is no longer exist in current context due to memory saving process is enable");
            }
            _codeExecutor = compilerSuite->generateCode(_compiledResult);
        }

        void evaluate() {
            if(!_compiledResult) compile();

            _compilationContext->startEvaluating();

            if (!_codeExecutor) generateCode();
            
            Context* context = Context::getCurrent();
            // additional variable' space for the expression
            int variableSpace = 0; // => no variable
            // space for expression itself require to run the code
            int requireSpaceToRunCode = _codeExecutor->getLocalSize();
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
            if(_evaluated) {
                throw std::runtime_error("Expresion has not been evaluated");
            }
            return _resultType;
        }

        virtual void onRequestUpdate() {
            _codeExecutor.reset();
        }
    };

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
}