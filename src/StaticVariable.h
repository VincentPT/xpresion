#pragma once
#include "DataType.h"
#include <string>

namespace xpression {
#pragma pack(push, 4)
    // transition object to set variable for an expression context
    struct Variable {
        const char* name;
        void* dataPtr;
        int dataSize;
        DataType type;
    };
#pragma pack(pop)

    
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

        void sync();
    };
}