#pragma once
#include <ScriptScope.h>
#include <Program.h>

namespace xpression {
    class ImmediateScope : public ffscript::ScriptScope {
    public:
        ImmediateScope(ffscript::ScriptCompiler* scriptCompiler);
        ~ImmediateScope();
        ffscript::Variable* findVariable(const std::string&);
        const wchar_t* parse(const wchar_t* text, const wchar_t* end);
	    int correctAndOptimize(ffscript::Program* program);
		bool extractCode(ffscript::Program* program);
    };
}