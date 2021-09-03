#pragma once
#include "StaticVariable.h"
#include "ScopedExpresionContext.h"
#include "VariableUpdater.h"
#include "ExpressionCpp.h"

namespace xpression {
    template <typename T>
    DataType typeFromCpp();

    template <class T>
    class XVariable {
        Variable _rawVariable;
        T _data;
        std::string _name;
    public:
        XVariable(const std::string& name, T data) : _name(name), _data(data) {
            _rawVariable.type = typeFromCpp<T>();
            _rawVariable.name = _name.c_str();
            _rawVariable.dataPtr = &_data;
            _rawVariable.dataSize = sizeof(T);
        }

        XVariable(const std::string& name) : _name(name) {
            _rawVariable.type = typeFromCpp<T>();
            _rawVariable.name = _name.c_str();
            _rawVariable.dataPtr = nullptr;
            _rawVariable.dataSize = sizeof(T);
        }

        virtual ~XVariable(){}

        Variable* get() {
            return &_rawVariable;
        }

        T& operator*() {
            return *(T*)_rawVariable.dataPtr;
        }

        const T& operator*() const {
            return *(T*)_rawVariable.dataPtr;
        }

        const std::string& name() {
            return _name;
        }

        void sync() {
            auto context = ScopedExpresionContext::current();
            if(context == nullptr) {
                throw std::runtime_error("There is no expression context in the variable'scope");
            }
            context->fillVariable(this->_name.c_str(), get());
        }
    };

    template <class T>
    class AutoVariable : public XVariable<T> {
        void autoRegistVariable() {
            auto context = ScopedExpresionContext::current();
            if(context == nullptr) {
                throw std::runtime_error("There is no expression context in the variable'scope");
            }
            context->addVariable(this->get());
        }
    public:
        AutoVariable(const std::string& name, T data) : XVariable<T>(name, data) {
            autoRegistVariable();
        }
        AutoVariable(const std::string& name) : XVariable<T>(name) {
            autoRegistVariable();
        }
        ~AutoVariable() {
            auto context = ScopedExpresionContext::current();
            context->removeVariable(this->get());
        }
    };

    // template <class T>
    // void XVariable<T>::sync() {
    //     auto context = ScopedExpresionContext::current();
    //     if(context == nullptr) {
    //         throw std::runtime_error("There is no expression context in the variable'scope");
    //     }
    //     context->fillVariable(this->_name.c_str(), get());
    // }

    template <class Ft>
    class FunctionalVariableUpdater : public VariableUpdater {
        Ft _fx;
    public:
        FunctionalVariableUpdater(FunctionalVariableUpdater&&) = delete;
        FunctionalVariableUpdater(const FunctionalVariableUpdater&) = delete;
        FunctionalVariableUpdater(Ft&& fx) : _fx(fx) {}
        bool onRequestUpdate(Variable* pVariable) {
            return _fx(pVariable);
        }
    };

    template <class Ft>
    void setVariableUpdater(ScopedExpresionContext* ctx, Ft&& fx) {
        ctx->setVariableUpdater(new FunctionalVariableUpdater<Ft>(std::move(fx)), true);
    }

    template <class Ft>
    void setVariableUpdater(ExpressionCpp* e, Ft&& fx) {
        e->setVariableUpdater(new FunctionalVariableUpdater<Ft>(std::move(fx)), true);
    }
}