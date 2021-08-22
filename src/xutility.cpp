
#include <BasicType.h>
#include "xutility.h"

using namespace ffscript;


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
        if(dynamicType == basicTypes.TYPE_DOUBLE) {
            return DataType::Double;
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

    int typeSize(DataType dt) {
        switch (dt)
        {
        case DataType::Boolean:
            return sizeof(bool);
        case DataType::Char:
            return sizeof(char);
        case DataType::Double:
            return sizeof(double);
        case DataType::Float:
            return sizeof(float);
        case DataType::Integer:
            return sizeof(int);
        case DataType::Long:
            return sizeof(long long);
        case DataType::Null:
            return sizeof(void*);
        case DataType::AsciiString:
            return sizeof(char*);
        case DataType::Void:
            return 0;
        case DataType::Wchar:
            return sizeof(wchar_t);
        case DataType::UnicodeString:
            return sizeof(wchar_t*);
        case DataType::String:
            return sizeof(wchar_t*);
        default:
            return -1;
        }
        return -1;
    }
}