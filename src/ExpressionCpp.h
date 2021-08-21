#pragma once
#include "xpression.h"

namespace xpression {
    class ExcutionContext;
    class InternalExpressionCpp;

    class EXPRESSIONCPP_API ExpressionCpp {
        InternalExpressionCpp* _pInternalExpresion;
        DataType _resultType;
    protected:
        void compile();
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