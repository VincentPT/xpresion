#include "ExpressionCpp.h"
#include "SimpleCompilerSuite.h"
#include "ExpressionContext.h"
#include "utility.h"

#include <exception>
#include <string.h>


namespace xpression {
    class InternalExpressionCpp {
        friend class ExpressionCpp;
        ExpressionContext* _compilationContext;
        ExpUnitExecutorRef _compiledResult;
        bool _compiled;
        wstring _expresionStr;
    public:
        InternalExpressionCpp(): _compiled(false) {}
        ~InternalExpressionCpp() {}
    };

    ExpressionCpp::ExpressionCpp(const wchar_t* expStr) : _resultType(DataType::Unknown) {
        _pInternalExpresion = new InternalExpressionCpp();
        _pInternalExpresion->_expresionStr = expStr;
    }

    ExpressionCpp::~ExpressionCpp() {
        delete _pInternalExpresion;
    }

    void ExpressionCpp::compile() {
        ExpressionContext* curentContext = ExpressionContext::getCurrentContext();
        if( curentContext == nullptr) {
            throw std::runtime_error("No instance of ExpressionContext for current thread found");
        }

        auto compilerSuite = curentContext->getCompilerSuite();
        auto& typeManager = compilerSuite->getTypeManager();
        auto& basicTypes = typeManager->getBasicTypes();

        auto compilerResult = compilerSuite->compileExpression(_pInternalExpresion->_expresionStr);

        auto dynamicReturnType = compilerResult->getInternalExpresion()->getRoot()->getReturnType().iType();
        _resultType = dynamicToStatic(basicTypes, dynamicReturnType);

        auto& executor = compilerResult->getExecutor();

        _pInternalExpresion->_compilationContext = curentContext;
        _pInternalExpresion->_compiledResult = executor;
        _pInternalExpresion->_compiled = true;
    }

    void ExpressionCpp::evaluate() {
        if(!_pInternalExpresion->_compiled) compile();
        auto& executor = _pInternalExpresion->_compiledResult;
        executor->runCode();
    }

    int ExpressionCpp::getResultInt() {
        throw std::runtime_error("Not implemeted");
    }

    bool ExpressionCpp::getResultBool() {
        throw std::runtime_error("Not implemeted");
    }

    float ExpressionCpp::getResultFloat() {
        throw std::runtime_error("Not implemeted");
    }

    double ExpressionCpp::getResultDouble() {
        throw std::runtime_error("Not implemeted");
    }

    long long ExpressionCpp::getResultLong() {
        throw std::runtime_error("Not implemeted");
    }

    const char* ExpressionCpp::getResultString() {
        auto& executor = _pInternalExpresion->_compiledResult;
        ((std::string*)executor->getReturnData())->c_str();
        throw std::runtime_error("Not implemeted");
    }

    const wchar_t* ExpressionCpp::getResultWString() {
        throw std::runtime_error("Not implemeted");
    }
}