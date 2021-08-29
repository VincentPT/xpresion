#pragma once
#include <Variable.h>
#include <Context.h>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <memory>

#include "VariableUpdater.h"

namespace xpression {
    class VariableManager {
        struct UpdateInfo {
            ffscript::Variable* pVariable;
            bool updated;
        };
        std::map<std::string, Variable*> _staticVariables;
        // variables are need to set data 
        std::list<UpdateInfo> _notSetDataVariables;
        VariableUpdater* _variableUpdateCallback;
        std::unique_ptr<VariableUpdater> _variableUpdateCallbackOwner;
        ffscript::Context* _variableContext;
    public:
        VariableManager(ffscript::Context* variableContext);
        ~VariableManager();

        void addVariable(Variable* pVariable);
        void removeVariable(Variable* pVariable);
        void addRequestUpdateVariable(ffscript::Variable* pScriptVariable, bool needAskUser);
        void requestUpdateVariables();
        void checkVariablesFullFilled();
        void setVariableUdater(VariableUpdater* pVariableUpdateCallback, bool deleteIt = false);
        VariableUpdater* getVariableUpdater();
    };
}