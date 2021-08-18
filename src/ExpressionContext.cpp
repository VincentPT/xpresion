#include "ExpressionContext.h"
#include "SimpleCompilerSuite.h"

namespace xpression {
#if _WIN32 || _WIN64
	__declspec(thread) ExpressionContext* _threadExpressionContext = nullptr;
// Check GCC
#elif __GNUC__
	__thread ExpressionContext* _threadExpressionContext = nullptr;
#endif

    ExpressionContext::ExpressionContext() {
        _pCompilerSuite = new SimpleCompilerSuite();
    }

    ExpressionContext::~ExpressionContext() {
        if(_pCompilerSuite) {
            delete _pCompilerSuite;
        }
    }

    SimpleCompilerSuite* ExpressionContext::getCompilerSuite() const {
        return _pCompilerSuite;
    }

    ExpressionContext* ExpressionContext::getCurrentContext() {
        return _threadExpressionContext;
    }

    void ExpressionContext::setCurrentContext(ExpressionContext* pContext) {
        _threadExpressionContext = pContext;
    }
}