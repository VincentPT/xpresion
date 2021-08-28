#include "ImmediateScope.h"
#include <exception>

using namespace ffscript;

namespace xpression {
    ImmediateScope::ImmediateScope(ffscript::ScriptCompiler* scriptCompiler) : ScriptScope(scriptCompiler) {}

    ImmediateScope::~ImmediateScope() {}

    Variable* ImmediateScope::findVariable(const std::string& name) {
        auto pVariable = ScriptScope::findVariable(name);


        return pVariable;
    }

    const wchar_t* ImmediateScope::parse(const wchar_t* text, const wchar_t* end) {
        throw std::runtime_error("ImmediateScope::parse is not implemented");
    }
    int ImmediateScope::correctAndOptimize(Program* program) {
        throw std::runtime_error("ImmediateScope::correctAndOptimize is not implemented");
    }
	bool ImmediateScope::extractCode(Program* program) {
        throw std::runtime_error("ImmediateScope::extractCode is not implemented");
    }
}

