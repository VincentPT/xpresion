#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testCustomScript1)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    scopedContext.setCustomScript(
        L"int sum(int a, int b) {\n"
        L"  return a + b;\n"
        L"}"
    );

    ExpressionCpp e(L"1 + sum(2, 3)");
    e.evaluate();

    //EXPECT_EQ(6, e.getResultInt());
}