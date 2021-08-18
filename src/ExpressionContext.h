#pragma once

namespace dynamicexp {
    class SimpleCompilerSuite;

    class ExpressionContext {
        SimpleCompilerSuite* _pCompilerSuite;
    public:
        ExpressionContext();
        ~ExpressionContext();

        SimpleCompilerSuite* getCompilerSuite() const;

        static ExpressionContext* getCurrentContext();
        static void setCurrentContext(ExpressionContext* pContext);
    };
}