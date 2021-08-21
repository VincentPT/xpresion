#include <gtest/gtest.h>
#include "xpression.h"

using namespace xpression;

TEST(ExpressionCpp, test1)
{
    ScopedExpresionContext scopedContext;
    ExpressionCpp e(L"1 + 2");
    e.evaluate();
}