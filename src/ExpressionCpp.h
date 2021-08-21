#pragma once
#include "xpression.h"

namespace xpression {
    class ExcutionContext;
    class InternalExpressionCpp;

    class EXPRESSIONCPP_API ExpressionCpp {
        InternalExpressionCpp* _pInternalExpresion;
    public:
        ExpressionCpp(const wchar_t* expstr);
        ~ExpressionCpp();
        
        void compile();
        void evaluate();
        int getResultInt();
        bool getResultBool();
        float getResultFloat();
        double getResultDouble();
        long long getResultLong();
        const wchar_t* getResultString();

        DataType getResultType() const;
    };

    EXPRESSIONCPP_API const char* getTypeName(DataType dt);
}