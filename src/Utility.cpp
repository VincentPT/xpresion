#include "Utility.h"

namespace xpression {
    DataType dynamicToStatic(const BasicTypes& basicTypes, int dynamicType) {
        if(dynamicType == basicTypes.TYPE_BOOL) {
            return DataType::Boolean;
        }
        if(dynamicType == basicTypes.TYPE_CHAR) {
            return DataType::Char;
        }
        if(dynamicType == basicTypes.TYPE_FLOAT) {
            return DataType::Float;
        }
        if(dynamicType == basicTypes.TYPE_INT) {
            return DataType::Integer;
        }
        if(dynamicType == basicTypes.TYPE_LONG) {
            return DataType::Long;
        }
        if(dynamicType == basicTypes.TYPE_NULL) {
            return DataType::Null;
        }
        if(dynamicType == basicTypes.TYPE_STRING) {
            return DataType::AsciiString;
        }
        if(dynamicType == basicTypes.TYPE_VOID) {
            return DataType::Void;
        }
        if(dynamicType == basicTypes.TYPE_WCHAR) {
            return DataType::Wchar;
        }
        if(dynamicType == basicTypes.TYPE_WSTRING) {
            return DataType::UnicodeString;
        }
        if(dynamicType == basicTypes.TYPE_RAWSTRING) {
            return DataType::String;
        }

        return DataType::Unknown;
    }

    int staticToDynamic(const BasicTypes& basicTypes, DataType staticType) {
        switch (staticType)
        {
        case DataType::Boolean:
            return basicTypes.TYPE_BOOL;
        case DataType::Char:
            return basicTypes.TYPE_CHAR;
        case DataType::Double:
            return basicTypes.TYPE_DOUBLE;
        case DataType::Float:
            return basicTypes.TYPE_FLOAT;
        case DataType::Integer:
            return basicTypes.TYPE_INT;
        case DataType::Long:
            return basicTypes.TYPE_LONG;
        case DataType::Null:
            return basicTypes.TYPE_NULL;
        case DataType::AsciiString:
            return basicTypes.TYPE_STRING;
        case DataType::Void:
            return basicTypes.TYPE_VOID;
        case DataType::Wchar:
            return basicTypes.TYPE_WCHAR;
        case DataType::UnicodeString:
            return basicTypes.TYPE_WSTRING;
        case DataType::String:
            return basicTypes.TYPE_RAWSTRING;
        default:
            return DATA_TYPE_UNKNOWN;
        }
        return DATA_TYPE_UNKNOWN;
    }
    template <>
    DataType typeFromCpp<bool>() {
        return DataType::Boolean;
    }
    template <>
    DataType typeFromCpp<int>() {
        return DataType::Integer;
    }

    template <>
    DataType typeFromCpp<long long>() {
        return DataType::Long;
    }

    template <>
    DataType typeFromCpp<double>() {
        return DataType::Double;
    }

    template <>
    DataType typeFromCpp<float>() {
        return DataType::Float;
    }
    template <>
    DataType typeFromCpp<char>() {
        return DataType::Char;
    }
    template <wchar_t>
    DataType typeFromCpp() {
        return DataType::Wchar;
    }
    template <>
    DataType typeFromCpp<std::string>() {
        return DataType::AsciiString;
    }
    template <>
    DataType typeFromCpp<std::wstring>() {
        return DataType::UnicodeString;
    }
}