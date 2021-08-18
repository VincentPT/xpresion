#include "ExpressionCpp.h"
#include "SimpleCompilerSuite.h"
#include "ExpressionContext.h"
#include "ExecutionContext.h"

#include "utility.h"

#include <exception>

namespace dynamicexp {
    ExpressionCpp::ExpressionCpp(const wchar_t* expStr) : _expStr(nullptr), _resultType(DataType::Unknown) {
    }

    ExpressionCpp::~ExpressionCpp() {
    }

    void ExpressionCpp::compiled() {
        ExpressionContext* curentContext = ExpressionContext::getCurrentContext();
        if( curentContext == nullptr) {
            throw std::runtime_error("No instance of ExpressionContext for current thread found");
        }

        auto compilerSuite = curentContext->getCompilerSuite();
        auto& typeManager = compilerSuite->getTypeManager();
        auto& basicTypes = typeManager->getBasicTypes();

        auto compilerResult = compilerSuite->compileExpression(_expStr);

        auto dynamicReturnType = compilerResult->getInternalExpresion()->getRoot()->getReturnType().iType();
        _resultType = dynamicToStatic(basicTypes, dynamicReturnType);

        auto& executor = compilerResult->getExecutor();

        if(_excutionContext) {
            delete _excutionContext;
        }

        _excutionContext = new ExcutionContext(curentContext, executor);
    }

    void ExpressionCpp::evaluate() {
        auto& executor = _excutionContext->getCompiledResult();
        executor->runCode();
    }

    int getResultInt() {
        throw std::runtime_error("Not implemeted");
    }

    bool getResultBool() {
        throw std::runtime_error("Not implemeted");
    }

    float getResultFloat() {
        throw std::runtime_error("Not implemeted");
    }

    double getResultDouble() {
        throw std::runtime_error("Not implemeted");
    }

    long long getResultLong() {
        throw std::runtime_error("Not implemeted");
    }

    const char* getResultString() {
        throw std::runtime_error("Not implemeted");
    }

    const wchar_t* getResultWString() {
        throw std::runtime_error("Not implemeted");
    }
}