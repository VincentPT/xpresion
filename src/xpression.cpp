#include "xpression.hpp"

namespace xpression {
    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<bool>() {
        return DataType::Boolean;
    }
    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<int>() {
        return DataType::Integer;
    }

    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<long long>() {
        return DataType::Long;
    }

    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<double>() {
        return DataType::Double;
    }

    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<float>() {
        return DataType::Float;
    }
    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<char>() {
        return DataType::Char;
    }
    template <wchar_t>
    EXPRESSIONCPP_API DataType typeFromCpp() {
        return DataType::Wchar;
    }
    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<std::string>() {
        return DataType::AsciiString;
    }
    template <>
    EXPRESSIONCPP_API DataType typeFromCpp<std::wstring>() {
        return DataType::UnicodeString;
    }
}