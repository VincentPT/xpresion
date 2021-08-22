#pragma once
#include "StaticVariable.h"
#include "VariableUpdater.h"

namespace ffscript {
    class CLamdaProg;
    class Program;
}

namespace xpression {
    class SimpleCompilerSuite;
    class VariableManager;
    
    enum class UserDataType {
        NotUsed,
        ScopedContext,
        // define more user data type here
    };

    struct UserData {
        void* data;
        UserDataType dt;
    };

    class ExpressionContext {
        SimpleCompilerSuite* _pCompilerSuite;
        ffscript::CLamdaProg* _pCustomScript;
        ffscript::Program* _pRawProgram;
        VariableManager* _pVariableManager;
        UserData   _userData;
    public:
        ExpressionContext();
        ~ExpressionContext();

        SimpleCompilerSuite* getCompilerSuite() const;
        void setCustomScript(const wchar_t* customScript);
        void startEvaluating();
        void stopEvaluating();
        void addVariable(Variable* pVariable);
        VariableUpdater* getVariableUpdater();
        void setVariableUpdater(VariableUpdater* pVariableUpdater);
        void setUserData(const UserData& userData);
        UserData& getUserData();

        static ExpressionContext* getCurrentContext();
        static void setCurrentContext(ExpressionContext* pContext);
    };
}