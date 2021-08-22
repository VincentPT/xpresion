#include <gtest/gtest.h>
#include "xpression.h"

#include <iostream>

using namespace xpression;

TEST(ExpressionCpp, testNormalExpression)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2");
    e.evaluate();
}

TEST(ExpressionCpp, testInvalidExpresion)
{
    // test evaluating an invalid expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + x");
    EXPECT_THROW(e.evaluate(), std::exception);
}

TEST(ExpressionCpp, testGetAsciiString)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"\"ascii string\"");
    e.evaluate();
    // ascii string will be convert to unicode string in getResultString function
    std::wcout << e.getResultString() << std::endl;
}

TEST(ExpressionCpp, testGetUnicodeString)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"L\"unicode string\"");
    e.evaluate();

    std::wcout << e.getResultString() << std::endl;
}

TEST(ExpressionCpp, testGetBuitIntString)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"L\"xscript'string \" + 1");
    e.evaluate();

    std::wcout << e.getResultString() << std::endl;
}

TEST(ExpressionCpp, testGetInt)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2");
    e.evaluate();

    EXPECT_EQ(3, e.getResultInt());
}

TEST(ExpressionCpp, testGetBoolean)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 == 2");
    e.evaluate();

    EXPECT_FALSE(e.getResultBool());
}

TEST(ExpressionCpp, testGetDouble)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2.0");
    e.evaluate();

    EXPECT_EQ(3.0, e.getResultDouble());
}

TEST(ExpressionCpp, testGetFloat)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2.0f");
    e.evaluate();

    EXPECT_EQ(3.0f, e.getResultFloat());
}

TEST(ExpressionCpp, testGetLong)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2ll");
    e.evaluate();

    EXPECT_EQ(3ll, e.getResultLong());
}

TEST(ExpressionCpp, testGetWrongResulType)
{
    // test evaluating a normal expression
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2ll");
    e.evaluate();
    EXPECT_THROW(e.getResultInt(), std::exception);
}