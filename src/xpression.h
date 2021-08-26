#pragma once

#ifdef _WIN32
#ifdef EXPORT_EXPRESSIONCPP
#define EXPRESSIONCPP_API __declspec(dllexport)
#else
#define EXPRESSIONCPP_API __declspec(dllimport)
#endif // EXPORT_EXPRESSIONCPP
#else
#define GAME_CONTROLLER_API
#endif // _WIN32

#include "DataType.h"
#include "ExpressionCpp.h"
#include "VariableUpdater.h"
#include "ScopedExpresionContext.h"
#include <exception>

namespace xpression {
    template <class T>
    class AutoVariable : public XVariable<T> {
        void autoRegistVariable() {
            auto context = ScopedExpresionContext::current();
            if(context == nullptr) {
                throw std::runtime_error("There is no expression context in the variable'scope");
            }
            context->addVariable(get());
        }
    public:
        AutoVariable(const std::string& name, T data) : XVariable(name, data) {
            autoRegistVariable();
        }
        AutoVariable(const std::string& name) : XVariable(name) {
            autoRegistVariable();
        }
        ~AutoVariable() {
            auto context = ScopedExpresionContext::current();
            context->removeVariable(get());
        }
    };
}