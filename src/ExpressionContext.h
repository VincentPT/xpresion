#pragma once

namespace ffscript {
    class CLamdaProg;
    class Program;
}

namespace xpression {
    class SimpleCompilerSuite;

    class ExpressionContext {
        SimpleCompilerSuite* _pCompilerSuite;
        ffscript::CLamdaProg* _pCustomScript;
        ffscript::Program* _pRawProgram;
    public:
        ExpressionContext();
        ~ExpressionContext();

        SimpleCompilerSuite* getCompilerSuite() const;
        void setCustomScript(const wchar_t* customScript);
        void startEvaluating();
        void stopEvaluating();

        static ExpressionContext* getCurrentContext();
        static void setCurrentContext(ExpressionContext* pContext);
    };
}