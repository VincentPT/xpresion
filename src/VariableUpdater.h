#pragma once
#include "StaticVariable.h"
#include "xpression.h"
#include "functional"

namespace xpression {
    class EXPRESSIONCPP_API VariableUpdater {
    public:
        VariableUpdater();
        virtual ~VariableUpdater();
        virtual bool onRequestUpdate(Variable* pVariable) = 0;
    };

    template <class Ft>
    class FunctionalVariableUpdater : public VariableUpdater {
        Ft _fx;
    public:
        FunctionalVariableUpdater(FunctionalVariableUpdater&&) = delete;
        FunctionalVariableUpdater(const FunctionalVariableUpdater&) = delete;
        FunctionalVariableUpdater(Ft&& fx) : _fx(fx) {}
        bool onRequestUpdate(Variable* pVariable) {
            return _fx(pVariable);
        }
    };
}