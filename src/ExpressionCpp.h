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

namespace xpression {
    class ExcutionContext;
    class InternalExpressionCpp;

    class EXPRESSIONCPP_API ExpressionCpp {
        InternalExpressionCpp* _pInternalExpresion;
        DataType _resultType;
    protected:
        void compiled();
    public:
        ExpressionCpp(const wchar_t* expstr);
        ~ExpressionCpp();
        
        void evaluate();
        int getResultInt();
        bool getResultBool();
        float getResultFloat();
        double getResultDouble();
        long long getResultLong();
        const char* getResultString();
        const wchar_t* getResultWString();

        DataType getResultType() const;
    };
}