#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testAutoVarialbe)
{
    ScopedExpresionContext scopedContext;
    AutoVariable<int> x("x", 2);
    
    ExpressionCpp e(L"1 + x");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}
