#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testCustomScript1)
{
    // test evaluating expression call and custom script
    ScopedExpresionContext scopedContext;
    scopedContext.setCustomScript(
        L"int sum(int a, int b) {\n"
        L"  return a + b;\n"
        L"}"
    );

    ExpressionCpp e(L"1 + sum(2, 3)");
    e.evaluate();

    EXPECT_EQ(6, e.getResultInt());
}

TEST(ExpressionCpp, testCustomScript2)
{
    // test evaluating expression call and custom script
    ScopedExpresionContext scopedContext;
    scopedContext.setCustomScript(
        L"int n = 4;\n"
        L"int sum(int a, int b) {\n"
        L"  return a + b + n;\n"
        L"}"
    );

    ExpressionCpp e(L"1 + sum(2, 3)");
    e.evaluate();

    EXPECT_EQ(10, e.getResultInt());
}

TEST(ExpressionCpp, testCustomScript3)
{
    // test run two expression in the same context
    ScopedExpresionContext scopedContext;
    scopedContext.setCustomScript(
        L"int sum(int a, int b) {\n"
        L"  return a + b;\n"
        L"}"
    );

    ExpressionCpp e1(L"1 + sum(2, 3)");
    e1.evaluate();
    ExpressionCpp e2(L"4 + sum(2, 3)");
    e2.evaluate();

    EXPECT_EQ(9, e2.getResultInt());
}

TEST(ExpressionCpp, testVaribleFromCustomScript)
{
    // test evaluating expression call and custom script
    ScopedExpresionContext scopedContext;
    scopedContext.setCustomScript(
        L"int n = 4;\n"
        L"int sum(int a, int b) {\n"
        L"  return a + b + n;\n"
        L"}"
    );

    XVariable<int> n("n");
    // sync variable with current expression context
    n.sync();

    // check value of variable after synchornizing
    EXPECT_EQ(4, *n);
}