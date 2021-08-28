#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testLocalVariable) {
    XVariable<double> x("x", 3.0);

    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + x");
    e.addVariable(x.get());
    e.evaluate();

    EXPECT_EQ(4.0, e.getResultDouble());
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