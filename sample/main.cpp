
#include "xpression.h"

#include <iostream>

using namespace xpression;
using namespace std;

const char* typeName(DataType dt) {
    switch (dt)
    {
    case DataType::Boolean:
        return "Boolean";
    case DataType::Char:
        return "Char";
    case DataType::Double:
        return "Double";
    case DataType::Float:
        return "Float";
    case DataType::Integer:
        return "Integer";
    case DataType::Long:
        return "Long";
    case DataType::Null:
        return "Null";
    case DataType::AsciiString:
        return "AsciiString";
    case DataType::Void:
        return "Void";
    case DataType::Wchar:
        return "Wchar";
    case DataType::UnicodeString:
        return "UnicodeString";
    case DataType::String:
        return "String";
    default:
        return "Unkown";
    }
}

class VariableUpdateMan : public xpression::VariableUpdater {
    int _dataX;
public:
    VariableUpdateMan() {
        _dataX = 2;
    }
    virtual bool onRequestUpdate(Variable* pVariable) {
        if(!strcmp(pVariable->name,"x")) {
            pVariable->dataPtr = &_dataX;
            return true;
        }
        return false;
    }
};

int main(int argc, char* argv[]) {

    // simple expression
    {
        ScopedExpresionContext scopedExpresionContext;
        const wchar_t* expStr = L"1 + 2";
        ExpressionCpp e(expStr);
        e.evaluate();
        
        auto returnType = e.getResultType();
        wcout << L"evaluate exp '" << expStr;
        cout << "' result = " << e.getResultInt() << " (" << typeName(returnType) << ")" << std::endl;
    }

    // expression with global variable
    {
        ScopedExpresionContext scopedExpresionContext;
        AutoVariable<int> x("x", 2);
        const wchar_t* expStr = L"1.0 + x";
        ExpressionCpp e(expStr);
        e.evaluate();
        
        auto returnType = e.getResultType();
        wcout << L"evaluate exp '" << expStr;
        cout << "' with " << x.name() << " = " << *x;
        cout << ", result = " << e.getResultDouble() << " (" << typeName(returnType) << ")" << endl;
    }

    // expression with local variable
    {
        ScopedExpresionContext scopedExpresionContext;
        XVariable<int> x("x", 3);
        const wchar_t* expStr = L"1.0 + x";
        ExpressionCpp e(expStr);
        e.addVariable(x.get());
        e.evaluate();
        
        auto returnType = e.getResultType();
        wcout << L"evaluate exp '" << expStr;
        cout << "' with " << x.name() << " = " << *x;
        cout << ", result = " << e.getResultDouble() << " (" << typeName(returnType) << ")" << endl;
    }

    // expression with custom script
    {
        // reversed stack size enough to run the custom script
        int stackSize = 256;
        ScopedExpresionContext scopedContext(stackSize);
        scopedContext.setCustomScript(
            L"int sum(int a, int b) {\n"
            L"  return a + b;\n"
            L"}"
        );
        const wchar_t* expStr = L"1 + sum(2, 3)";
        ExpressionCpp e(expStr);
        e.evaluate();

        auto returnType = e.getResultType();
        wcout << L"evaluate exp '" << expStr;
        cout << "' result = " << e.getResultInt() << " (" << typeName(returnType) << ")" << endl;
    }

    // get impcilit variables from script
    {
        // reversed stack size enough to run the custom script
        int stackSize = 256;
        ScopedExpresionContext scopedContext(stackSize);
        scopedContext.setCustomScript(
            L"int n = 1;\n"
            L"int sum(int a, int b) {\n"
            L"  n = a + b;\n"
            L"  return n;\n"
            L"}"
        );

        // get variable 'n' from global context
        XVariable<int> n("n", 0);
        n.sync();

        cout << "before evaluate '" << n.name() << "' = " << *n << endl;

        const wchar_t* expStr = L"m = 1 + sum(2, 3)";
        ExpressionCpp e(expStr);
        e.evaluate();

        // get variable 'n' from local context
        // the local variable should retreive immediately after evaluate expression
        XVariable<int> m("m", 0);
        e.fillVariable(m.name().c_str(), m.get());

        cout << "after evaluate '" << n.name() << "' = " << *n << endl;
        cout << "after evaluate '" << m.name() << "' = " << *m << endl;
    }

    // delay set value for global variable
    {
        ScopedExpresionContext scopedContext;

        // variable 'x' don't need to provide value at declaration
        AutoVariable<int> x("x");

        // set up delay update for variable in current context
        VariableUpdateMan delayUpdater;
        scopedContext.setVariableUpdater(&delayUpdater);

        const wchar_t* expStr = L"1 + x";
        ExpressionCpp e(expStr);

        // delay update will run before expression is evaluated
        e.evaluate();

        auto returnType = e.getResultType();
        wcout << L"evaluate exp '" << expStr;
        cout << "' result = " << e.getResultInt() << " (" << typeName(returnType) << ")" << endl;

        cout << "after evaluate '" << x.name() << "' = " << *x << endl;
    }

    return 0;
}