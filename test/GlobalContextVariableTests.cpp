#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testExpresionWithNormalVarialbe)
{
    ScopedExpresionContext scopedContext;
    XVariable<int> x("x", 2);
    scopedContext.addVariable(x.get());
    
    ExpressionCpp e(L"1 + x");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}

class VariableUpdateMan : public xpression::VariableUpdater {
    int _dataX;
public:
    VariableUpdateMan() {
        _dataX = 2;
    }
    virtual bool onRequestUpdate(Variable* pVariable) {
        if(!strcmp(pVariable->name,"x")) {
            pVariable->dataPtr = &_dataX;
            return true;
        }
        return false;
    }
};

TEST(ExpressionCpp, testExpresionWithDelayUpdateVarialbe)
{
    ScopedExpresionContext scopedContext;
    XVariable<int> x("x");
    scopedContext.addVariable(x.get());

    VariableUpdateMan delayUpdater;
    scopedContext.setVariableUpdater(&delayUpdater);

    ExpressionCpp e(L"1 + x");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}


TEST(ExpressionCpp, testExpresionWithDelayUpdateVarialbeNotFound)
{
    ScopedExpresionContext scopedContext;
    XVariable<int> x("y");
    scopedContext.addVariable(x.get());

    VariableUpdateMan delayUpdater;
    scopedContext.setVariableUpdater(&delayUpdater);

    ExpressionCpp e(L"1 + y");
    EXPECT_THROW(e.evaluate(), std::exception);
}
