#include "Utility.h"

namespace dynamicexp {
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
            return DataType::String;
        }
        if(dynamicType == basicTypes.TYPE_VOID) {
            return DataType::Void;
        }
        if(dynamicType == basicTypes.TYPE_WCHAR) {
            return DataType::Wchar;
        }
        if(dynamicType == basicTypes.TYPE_WSTRING) {
            return DataType::Wstring;
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
        case DataType::String:
            return basicTypes.TYPE_STRING;
        case DataType::Void:
            return basicTypes.TYPE_VOID;
        case DataType::Wchar:
            return basicTypes.TYPE_WCHAR;
        case DataType::Wstring:
            return basicTypes.TYPE_WSTRING;
        default:
            return DATA_TYPE_UNKNOWN;
        }
        return DATA_TYPE_UNKNOWN;
    }

    DataType cppToStatic(bool) {
        return DataType::Boolean;
    }

    DataType cppToStatic(int) {
        return DataType::Integer;
    }

    DataType cppToStatic(long long) {
        return DataType::Long;
    }

    DataType cppToStatic(double) {
        return DataType::Double;
    }

    DataType cppToStatic(float) {
        return DataType::Float;
    }

    DataType cppToStatic(char) {
        return DataType::Char;
    }

    DataType cppToStatic(wchar_t) {
        return DataType::Wchar;
    }

    DataType cppToStatic(const std::string&) {
        return DataType::String;
    }

    DataType cppToStatic(const std::wstring&) {
        return DataType::Wstring;
    }
}