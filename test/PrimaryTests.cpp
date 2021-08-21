#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, test1)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2");
    e.evaluate();
}

TEST(ExpressionCpp, test2)
{
    // test evaluating an invalid expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + x");
    EXPECT_THROW(e.evaluate(), std::exception);
}

TEST(ExpressionCpp, test3)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"\"ascii string\"");
    e.evaluate();
    // ascii string will be convert to unicode string in getResultString function
    std::wcout << e.getResultString() << std::endl;
}

TEST(ExpressionCpp, test4)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"L\"unicode string\"");
    e.evaluate();

    std::wcout << e.getResultString() << std::endl;
}

TEST(ExpressionCpp, test5)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"L\"xscript'string \" + 1");
    e.evaluate();

    std::wcout << e.getResultString() << std::endl;
}