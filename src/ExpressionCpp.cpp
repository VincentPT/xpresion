#include "ExpressionCpp.h"
#include "SimpleCompilerSuite.h"
#include "ExpressionContext.h"
#include "utility.h"
#include <Utils.h>

#include <exception>
#include <string.h>
#include <memory>

namespace xpression {
    class InternalExpressionCpp {
        friend class ExpressionCpp;
        ExpressionContext* _compilationContext;
        ExpUnitExecutorRef _compiledResult;
        wstring _expresionStr;
        bool _evaluated;
        DataType _resultType;
        typedef std::unique_ptr<void, std::function<void(void*)>> ResultDeletor;
        ResultDeletor _manualDeletedResult;
    public:
        InternalExpressionCpp(): _evaluated(false), _resultType(DataType::Unknown)  {}
        ~InternalExpressionCpp() {}

        void compile() {
            ExpressionContext* curentContext = ExpressionContext::getCurrentContext();
            if( curentContext == nullptr) {
                throw std::runtime_error("No instance of ExpressionContext for current thread found");
            }

            auto compilerSuite = curentContext->getCompilerSuite();
            auto& typeManager = compilerSuite->getTypeManager();
            auto& basicTypes = typeManager->getBasicTypes();

            auto compilerResult = compilerSuite->compileExpression(_expresionStr);

            auto dynamicReturnType = compilerResult->getInternalExpresion()->getRoot()->getReturnType().iType();
            _resultType = dynamicToStatic(basicTypes, dynamicReturnType);

            auto& executor = compilerResult->getExecutor();

            _compilationContext = curentContext;
            _compiledResult = executor;
        }

        void evaluate() {
            if(!_compiledResult) compile();
            _compiledResult->runCode();
            _evaluated = true;

            if(_resultType == DataType::String) {
                _manualDeletedResult = ResultDeletor(
                    _compiledResult->getReturnData(),
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
            if(typeFromCpp<T>() != _resultType) {
                throw std::runtime_error(std::string("expression result is not ") + typeid(T).name());
            }
            return *(T*)_compiledResult->getReturnData();
        }

        const wchar_t* getResultString() {
            if(!_evaluated) {
                throw std::runtime_error("Expresion has not been evaluated");
            }
            if(DataType::String == _resultType) {
                return ((RawString*)_compiledResult->getReturnData())->elms;
            }
            if(DataType::UnicodeString == _resultType) {
                return (*((std::wstring**)_compiledResult->getReturnData()))->c_str();
            }
            if(DataType::AsciiString == _resultType) {
                auto pRes = *((std::string**)_compiledResult->getReturnData());
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
    };

    ExpressionCpp::ExpressionCpp(const wchar_t* expStr) {
        _pInternalExpresion = new InternalExpressionCpp();
        _pInternalExpresion->_expresionStr = expStr;
    }

    ExpressionCpp::~ExpressionCpp() {
        delete _pInternalExpresion;
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