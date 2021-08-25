#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testVarialbeWithScript1)
{
    // add variable after set custom script
    ScopedExpresionContext scopedContext;
    scopedContext.setCustomScript(
        L"int sum(int a, int b) {\n"
        L"  return a + b;\n"
        L"}"
    );

    AutoVariable<int> x("x", 2);
    
    ExpressionCpp e(L"1 + sum(x,3)");
    e.evaluate();

    EXPECT_EQ(6, e.getResultInt());
}

TEST(ExpressionCpp, testVarialbeWithScript2)
{
    // add variable before set custom script
    ScopedExpresionContext scopedContext;
    AutoVariable<int> x("x", 2);
    scopedContext.setCustomScript(
        L"int sum(int a, int b) {\n"
        L"  return a + b;\n"
        L"}"
    );
    
    ExpressionCpp e(L"1 + sum(x,3)");
    e.evaluate();

    EXPECT_EQ(6, e.getResultInt());
}

TEST(ExpressionCpp, testVarialbeWithScript3)
{
    // add variable before set custom script
    ScopedExpresionContext scopedContext;
    AutoVariable<int> x("x", 2);
    scopedContext.setCustomScript(
        L"int n = 1;\n"
        L"int sum(int a, int b) {\n"
        L"  return a + b + n;\n"
        L"}"
    );
    
    ExpressionCpp e(L"1 + sum(x,3)");
    e.evaluate();

    EXPECT_EQ(7, e.getResultInt());
}

TEST(ExpressionCpp, testVarialbeWithScript4)
{
    // add variable before set custom script
    ScopedExpresionContext scopedContext;
    AutoVariable<int> x("x", 2);
    AutoVariable<int> y("y", 3);
    scopedContext.setCustomScript(
        L"int n = 1;\n"
        L"int sum(int a, int b) {\n"
        L"  return a + b + n;\n"
        L"}"
    );
    
    ExpressionCpp e(L"1 + sum(x,3)");
    e.evaluate();

    EXPECT_EQ(7, e.getResultInt());
}

TEST(ExpressionCpp, testVarialbeWithScript5)
{
    // add variable before set custom script
    ScopedExpresionContext scopedContext;
    scopedContext.setCustomScript(
        L"int n = 1;\n"
        L"int sum(int a, int b) {\n"
        L"  return a + b + n;\n"
        L"}"
    );
    
    AutoVariable<int> x("x", 2);
    AutoVariable<int> y("y", 3);

    ExpressionCpp e(L"1 + sum(x,3)");
    e.evaluate();

    EXPECT_EQ(7, e.getResultInt());
}
