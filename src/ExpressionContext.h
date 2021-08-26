#pragma once
#include "StaticVariable.h"
#include "VariableUpdater.h"

namespace ffscript {
    class Program;
}

namespace xpression {
    class SimpleCompilerSuite;
    class VariableManager;
    class ExpressionEventManager;
    
    enum class UserDataType {
        NotUsed,
        ScopedContext,
        // define more user data type here
    };

    struct UserData {
        void* data;
        UserDataType dt;
    };

    class ExpressionEventHandler {
    public:
        virtual void onRequestUpdate() = 0;
    };

    class ExpressionContext {
        SimpleCompilerSuite* _pCompilerSuite; 
        ffscript::Program* _pRawProgram;
        VariableManager* _pVariableManager;
        UserData   _userData;
        ExpressionEventManager* _eventManager;
        int _allocatedDataSize;
        int _allocatedScopeSize;
        bool _needUpdateVariable;
        bool _needRunGlobalCode;
        bool _needRegenerateCode;
    public:
        ExpressionContext();
        ~ExpressionContext();

        SimpleCompilerSuite* getCompilerSuite() const;
        void setCustomScript(const wchar_t* customScript);
        void startEvaluating();
        void addVariable(Variable* pVariable);
        void removeVariable(Variable* pVariable);
        VariableUpdater* getVariableUpdater();
        void setVariableUpdater(VariableUpdater* pVariableUpdater);
        void setUserData(const UserData& userData);
        UserData& getUserData();
        void addExpressionEventHandler(ExpressionEventHandler* handler);
        void removeExpressionEventHandler(ExpressionEventHandler* handler);

        static ExpressionContext* getCurrentContext();
        static void setCurrentContext(ExpressionContext* pContext);
    };
}