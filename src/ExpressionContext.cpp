#include "ExpressionContext.h"
#include "SimpleCompilerSuite.h"
#include <string.h>
#include <DefaultPreprocessor.h>
#include <CLamdaProg.h>
#include <Program.h>

namespace xpression {
#if _WIN32 || _WIN64
	__declspec(thread) ExpressionContext* _threadExpressionContext = nullptr;
// Check GCC
#elif __GNUC__
	__thread ExpressionContext* _threadExpressionContext = nullptr;
#endif

    ExpressionContext::ExpressionContext() : _pRawProgram(nullptr), _pCustomScript(nullptr) {
        _pCompilerSuite = new SimpleCompilerSuite();
        _pCompilerSuite->setPreprocessor(std::make_shared<DefaultPreprocessor>());
    }

    ExpressionContext::~ExpressionContext() {
        if(_pCompilerSuite) {
            delete _pCompilerSuite;
        }
        stopEvaluating();
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

    void ExpressionContext::setCustomScript(const wchar_t* customScript) {
        if(_pRawProgram || _pCustomScript) {
             throw std::runtime_error("custom script is already available");
        }
        auto len = wcslen(customScript);
        _pRawProgram = _pCompilerSuite->compileProgram(customScript, customScript + len);
        _pCustomScript = nullptr;

        // check if the compiling process is failed...
        if (_pRawProgram == nullptr) {
            // ...then get error information
            // then shows to user
            int line, column;
            _pCompilerSuite->getLastCompliedPosition(line, column);

            string errorMsg("error at line = ");
            errorMsg.append(std::to_string(line + 1));
            errorMsg.append(", column = ");
            errorMsg.append(std::to_string(column + 1));
            errorMsg.append(" msg: ");
            errorMsg.append(_pCompilerSuite->getCompiler()->getLastError());
            
            throw std::runtime_error(errorMsg);
        }
    }

    void ExpressionContext::startEvaluating() {
        // no need to prepare for evaluating
        if(_pRawProgram == nullptr) return;

        // evaluating is already start
        if(_pCustomScript) return;
        
        _pCustomScript = _pCompilerSuite->detachProgram(_pRawProgram);
        _pCustomScript->runGlobalCode();

#ifdef MEMORY_SAVING
        // compiler suite is no longer necessary
        delete _pCompilerSuite;
        _pCompilerSuite = nullptr;
#endif
    }

    void ExpressionContext::stopEvaluating() {
        if(_pRawProgram) {
            delete _pRawProgram;
        }
        if(_pCustomScript) {
            _pCustomScript->cleanupGlobalMemory();
            delete _pCustomScript;
        }
    }
}