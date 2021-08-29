#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testLocalVariable1) {
    XVariable<double> x("x", 3.0);

    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + x");
    e.addVariable(x.get());
    e.evaluate();

    EXPECT_EQ(4.0, e.getResultDouble());
}

TEST(ExpressionCpp, testLocalVariable2) {
    ScopedExpresionContext scopedExpresionContext;
    XVariable<int> x("x", 3);
    std::wstring expStr = L"1.0 + x";
    ExpressionCpp e(expStr.c_str());
    e.addVariable(x.get());
    e.evaluate();

    EXPECT_EQ(4.0, e.getResultDouble());
}

TEST(ExpressionCpp, testLocalVariableDelayDeclaration) {
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + x");

    int variableDataInSomeWhere = 0;
    setVariableUpdater(&e, [&variableDataInSomeWhere](Variable* pVariable){
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

    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}

TEST(ExpressionCpp, testLocalVariableWithScript) {
    XVariable<double> x("x", 3.0);

    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    ExpressionCpp e(L"1 + sum(x, 2.0)");
    e.addVariable(x.get());
    e.evaluate();

    EXPECT_EQ(7.0, e.getResultDouble());
}

TEST(ExpressionCpp, testGetLocalVariable) {
    XVariable<double> x("x", 0.0);

    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    ExpressionCpp e(L"x = sum(1.0, 2.0)");
    e.addVariable(x.get());
    e.evaluate();

    EXPECT_EQ(4.0, *x);
}

TEST(ExpressionCpp, testMixLocalGlobalVariables1) {
    XVariable<double> x("x", 2.0);

    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    // variable 'x' is understood as global variable
    ExpressionCpp e2(L"sum(x, 2.0)");
    e2.evaluate();
    EXPECT_EQ(4.0, e2.getResultDouble());

    ExpressionCpp e1(L"sum(x, 2.0)");
    // variable 'x' is now understood as local variable
    e1.addVariable(x.get());
    e1.evaluate();

    EXPECT_EQ(5.0, e1.getResultDouble());
}

TEST(ExpressionCpp, testMixLocalGlobalVariables2) {
    XVariable<double> x("x", 2.0);

    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    ExpressionCpp e1(L"sum(x, 2.0)");
    // variable 'x' is now understood as local variable
    e1.addVariable(x.get());
    e1.evaluate();

    EXPECT_EQ(5.0, e1.getResultDouble());

    // variable 'x' is understood as global variable
    ExpressionCpp e2(L"sum(x, 2.0)");
    e2.evaluate();
    EXPECT_EQ(4.0, e2.getResultDouble());
}

TEST(ExpressionCpp, testFillLocalVariable1) {
    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    
    XVariable<double> x("x", 0.0);
    ExpressionCpp e1(L"x = 2");
    // variable 'x' is now understood as local variable
    e1.addVariable(x.get());
    e1.evaluate();

    // fill local varible by expression object
    e1.fillVariable(x.get()->name, x.get());

    EXPECT_EQ(2, *x);
}

TEST(ExpressionCpp, testFillLocalVariable2) {
    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    
    XVariable<double> x("x", 0.0);
    ExpressionCpp e1(L"y = 2");
    e1.evaluate();

    // fill global varible by expression object
    e1.fillVariable(x.get()->name, x.get());

    EXPECT_EQ(1, *x);
}

TEST(ExpressionCpp, testFillLocalVariable3) {
    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    XVariable<double> y("y", 0.0);
    ExpressionCpp e1(L"y = 2.0");
    e1.evaluate();

     // fill auto local varible by expression object
    e1.fillVariable(y.get()->name, y.get());

    EXPECT_EQ(2.0, *y);
}

TEST(ExpressionCpp, testFillVariablesMix) {
    ScopedExpresionContext scopedContext(256);
    scopedContext.setCustomScript(
        L"double x = 1;\n"
        L"double sum(double a, double b) {\n"
        L"  return a + b + x;\n"
        L"}"
    );

    XVariable<double> y("y", 0.0);
    XVariable<double> x("x", 0.0);
    ExpressionCpp e1(L"y = 2.0");
    e1.evaluate();

     // fill auto local varible by expression object
    e1.fillVariable(y.get()->name, y.get());
    e1.fillVariable(x.get()->name, x.get());

    EXPECT_EQ(2.0, *y);
    EXPECT_EQ(1.0, *x);
}