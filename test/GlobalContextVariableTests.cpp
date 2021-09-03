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

TEST(ExpressionCpp, testExpresionWithDelayUpdateVarialbe)
{
    ScopedExpresionContext scopedContext;
    XVariable<int> x("x");
    scopedContext.addVariable(x.get());

    int variableDataInSomeWhere = 0;
    setVariableUpdater(&scopedContext, [&variableDataInSomeWhere](Variable* pVariable){
        if(!strcmp(pVariable->name,"x")) {
            variableDataInSomeWhere = 2;
            pVariable->dataPtr = &variableDataInSomeWhere;
            return true;
        }
        return false;
    });

    ExpressionCpp e(L"1 + x");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}


TEST(ExpressionCpp, testExpresionWithDelayUpdateVarialbeNotFound)
{
    ScopedExpresionContext scopedContext;
    XVariable<int> x("y");
    scopedContext.addVariable(x.get());

    int variableDataInSomeWhere = 0;
    setVariableUpdater(&scopedContext, [&variableDataInSomeWhere](Variable* pVariable){
        if(!strcmp(pVariable->name,"x")) {
            variableDataInSomeWhere = 2;
            pVariable->dataPtr = &variableDataInSomeWhere;
            return true;
        }
        return false;
    });

    ExpressionCpp e(L"1 + y");
    EXPECT_THROW(e.evaluate(), std::exception);
}

TEST(ExpressionCpp, testExpresionWithDelayDeclareVarialbe)
{
    ScopedExpresionContext scopedContext;
    int variableDataInSomeWhere = 0;
    setVariableUpdater(&scopedContext, [&variableDataInSomeWhere](Variable* pVariable){
        if(!strcmp(pVariable->name,"x")) {
            variableDataInSomeWhere = 2;
            // delay declaration need to specify type
            // variable type need while compiling script
            pVariable->type = DataType::Integer;
            // delay updation need to specify data
            // variable data need while evaluating script
            pVariable->dataPtr = &variableDataInSomeWhere;
            return true;
        }
        return false;
    });

    ExpressionCpp e(L"1 + x");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}

TEST(ExpressionCpp, testExpresionWithDelayDeclareVarialbeWithScript)
{
    ScopedExpresionContext scopedContext;
    int variableDataInSomeWhere = 0;
    setVariableUpdater(&scopedContext, [&variableDataInSomeWhere](Variable* pVariable){
        if(!strcmp(pVariable->name,"x")) {
            variableDataInSomeWhere = 2;
            // delay declaration need to specify type
            // variable type need while compiling script
            pVariable->type = DataType::Integer;
            // delay updation need to specify data
            // variable data need while evaluating script
            pVariable->dataPtr = &variableDataInSomeWhere;
            return true;
        }
        return false;
    });

    scopedContext.setCustomScript(
        L"int getX() {\n"
        L"  return x;\n"
        L"}"
    );

    ExpressionCpp e(L"getX()");
    e.evaluate();

    EXPECT_EQ(variableDataInSomeWhere, e.getResultInt());
}

TEST(ExpressionCpp, testExpresionWithDelayDeclareVarialbeFailed)
{
    ScopedExpresionContext scopedContext;
    int variableDataInSomeWhere = 0;
    setVariableUpdater(&scopedContext, [&variableDataInSomeWhere](Variable* pVariable){
        if(!strcmp(pVariable->name,"x")) {
            variableDataInSomeWhere = 2;
            pVariable->type = DataType::Integer;
            pVariable->dataPtr = &variableDataInSomeWhere;
            return true;
        }
        return false;
    });

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

    XVariable<double> y("y", 5.0);
    scopedContext.addVariable(y.get());

    ExpressionCpp e2(L"1 + y");
    e2.evaluate();
    EXPECT_EQ(6.0, e2.getResultDouble());

    e1.evaluate();
    EXPECT_EQ(3, e1.getResultInt());

    e2.evaluate();
    EXPECT_EQ(6.0, e2.getResultDouble());
}