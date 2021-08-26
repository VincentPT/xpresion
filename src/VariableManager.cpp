#include "VariableManager.h"
#include <ScriptScope.h>
#include <string.h>
#include "xutility.h"

namespace xpression {
    VariableManager::VariableManager(ffscript::Context* variableContext):_variableContext(variableContext) , _variableUpdateCallback(nullptr) {
        if(!variableContext) {
            throw std::runtime_error("Internal error: variableContext is null");
        }
    }
    VariableManager::~VariableManager() {}

    void VariableManager::addVariable(Variable* pVariable) {
        auto it = _staticVariables.insert(std::pair<std::string, Variable*>(pVariable->name, pVariable));
        if (it.second == false) {
			throw std::runtime_error("Duplicate variable");
		}
    }

    void VariableManager::removeVariable(Variable* pVariable) {
        auto it = _staticVariables.find(pVariable->name);
        if (it != _staticVariables.end()) {
			_staticVariables.erase(it);
		}
        _notSetDataVariables.remove_if([pVariable](const UpdateInfo& info){
            return !strcmp(pVariable->name, info.pVariable->getName().c_str());
        });
    }

    void VariableManager::addRequestUpdateVariable(ffscript::Variable* pScriptVariable, bool needAskUser) {
        UpdateInfo updateInfo = {pScriptVariable, !needAskUser};
        _notSetDataVariables.push_back(updateInfo);
    }

    void VariableManager::requestUpdateVariables() {

        for(UpdateInfo& updateInfo : _notSetDataVariables) {
            auto it = _staticVariables.find(updateInfo.pVariable->getName());
            if(it == _staticVariables.end()) {
                throw std::runtime_error("Internal error: variable is not cached");
            }
            auto staticVariable = it->second;
            if(!updateInfo.updated && _variableUpdateCallback) {
                updateInfo.updated = _variableUpdateCallback->onRequestUpdate(staticVariable) && staticVariable->dataPtr != nullptr;
            }
           
            if(updateInfo.updated) {
                auto pVariable = updateInfo.pVariable;
                void* variableDataAddressDst =
                    _variableContext->getAbsoluteAddress(_variableContext->getCurrentOffset() + pVariable->getOffset());

                if(typeSize(staticVariable->type) != staticVariable->dataSize) {
                    throw std::runtime_error("malformed variable data");
                }
                if(pVariable->getSize() != staticVariable->dataSize) {
                    throw std::runtime_error("type size mismatch");
                }

                memcpy_s(variableDataAddressDst, pVariable->getSize(), staticVariable->dataPtr, staticVariable->dataSize);
                staticVariable->dataPtr = variableDataAddressDst;
            }
        }
    }

    void VariableManager::checkVariablesFullFilled() {
        for(UpdateInfo& updateInfo : _notSetDataVariables) {
            if(!updateInfo.updated) {
                throw std::runtime_error("variable '" + updateInfo.pVariable->getName() + "' is not set");
            }
        }
    }

    VariableUpdater* VariableManager::getVariableUpdater() {
        return _variableUpdateCallback;
    }

    void VariableManager::setVariableUdater(VariableUpdater* pVariableUpdateCallback) {
        _variableUpdateCallback = pVariableUpdateCallback;
    }
}