#include "ExpressionGlobalScope.h"
#include "ExpressionInternal.h"
#include "xutility.h"

using namespace ffscript;

namespace xpression {
    ExpressionGlobalScope::ExpressionGlobalScope(ExpressionContext* context, int stackSize, ScriptCompiler* scriptCompiler) : 
        _context(context), GlobalScope(stackSize, scriptCompiler) {
    }

    ExpressionGlobalScope::~ExpressionGlobalScope() {}

    ffscript::Variable* ExpressionGlobalScope::findVariable(const std::string& name) {
        auto pVariable = GlobalScope::findVariable(name);

        if(pVariable) return pVariable;

        auto variableUpdater =_context->getVariableUpdater();
        if(variableUpdater == nullptr) return nullptr;

        ffscript::Variable* result = nullptr;
        _variableContainer.push_back({0});
        xpression::Variable& variable = _variableContainer.back();

        RaiiScopeExecutor autoCheckResult([&](){
            if(result) {
                // update variable name
                // point variable name to an allocated buffer
                variable.name = result->getName().c_str();
            }
            else {
                // remove the temporary variable due to unsuccessful operation
                _variableContainer.pop_back();
            }
        });
        
        variable.name = name.c_str();
        if(!variableUpdater->onRequestUpdate(&variable)) {
            return nullptr;
        }
        if(variable.type == DataType::Unknown) {
            throw std::runtime_error("the variable '" + name + "' need to specify type when the script is being compiled");
        }
        variable.dataSize = typeSize(variable.type);
        result = _context->addVariable(&variable);
        return result;
    }
}