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

TEST(ExpressionCpp, testAddVariableTwoTimes)
{
    ScopedExpresionContext scopedContext;
    XVariable<int> x("x", 2);
    scopedContext.addVariable(x.get());

    ExpressionCpp e1(L"1 + x");
    e1.evaluate();
    EXPECT_EQ(3, e1.getResultInt());

    XVariable<double> y("y", 3.0);
    scopedContext.addVariable(y.get());

    ExpressionCpp e2(L"1 + y");
    e2.evaluate();
    EXPECT_EQ(4.0, e2.getResultDouble());
}

TEST(ExpressionCpp, testAddVariableAndRunTwoTimes)
{
    ScopedExpresionContext scopedContext;
    XVariable<int> x("x", 2);
    scopedContext.addVariable(x.get());

    ExpressionCpp e1(L"1 + x");
    e1.evaluate();
    EXPECT_EQ(3, e1.getResultInt());

    XVariable<double> y("y", 3.0);
    scopedContext.addVariable(y.get());

    ExpressionCpp e2(L"1 + y");
    e2.evaluate();
    EXPECT_EQ(4.0, e2.getResultDouble());

    e1.evaluate();
    EXPECT_EQ(3, e1.getResultInt());

    e2.evaluate();
    EXPECT_EQ(4.0, e2.getResultDouble());
}