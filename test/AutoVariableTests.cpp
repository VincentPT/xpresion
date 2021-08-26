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

TEST(ExpressionCpp, testUpdateAutoVariable)
{
    ScopedExpresionContext scopedContext;
    AutoVariable<int> x("x", 2);
    
    ExpressionCpp e(L"1 + x");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());

    // update variale to new value and evaluate expression again
    *x = 10;
    e.evaluate();
    EXPECT_EQ(11, e.getResultInt());
}

TEST(ExpressionCpp, testAutoRemoveAutoVariable)
{
    ScopedExpresionContext scopedContext;
    AutoVariable<int> x("x", 2);
    {
        auto x = new AutoVariable<int>("y", 2);
        delete x;
    }
    ExpressionCpp e(L"1 + x");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}

