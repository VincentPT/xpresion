# Introduce
This is a C++ Expression calculator library.
This project utilizes [ffscript](https://github.com/VincentPT/ffscript) library - a script compiler project - to make usage of expression calculation with ease.
The project include [ffscript](https://github.com/VincentPT/ffscript) as a submodule. So, also remember to clone [ffscript](https://github.com/VincentPT/ffscript) module.

For git 1.6.5 or higher, you can use below command to clone the whole project (including submodule)  
```git clone --recursive https://github.com/VincentPT/xpression.git```

# Usage
```
#include "xpression.h"
#include <iostream>

using namespace xpression;
using namespace std;
  ...
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
        ...
    }
    
    // expression with local variable
    {
        ScopedExpresionContext scopedExpresionContext;
        XVariable<int> x("x", 3);
        const wchar_t* expStr = L"1.0 + x";
        ExpressionCpp e(expStr);
        e.addVariable(x.get());
        e.evaluate();
        ...
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
        ...
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

        // get variable 'm' from local context
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
        int variableDataInSomeWhere = 2;
        setVariableUpdater([&variableDataInSomeWhere](Variable* pVariable){
            if(!strcmp(pVariable->name,"x")) {
                variableDataInSomeWhere = 2;
                pVariable->dataPtr = &variableDataInSomeWhere;
                return true;
            }
            return false;
        });

        const wchar_t* expStr = L"1 + x";
        ExpressionCpp e(expStr);

        // delay update will run before expression is evaluated
        e.evaluate();
        ...
    }
    
    // delay global & local variables declaration
    {
        ScopedExpresionContext scopedContext(1024);

        int x, y, z;
        setVariableUpdater([&](Variable* pVariable){
            if(!strcmp(pVariable->name,"x")) {
                x = 1;
                // delay declaration need to specify type.
                // variable type need while compiling script.
                pVariable->type = DataType::Integer;
                // delay updation need to specify data.
                // variable data need while evaluating script.
                pVariable->dataPtr = &x;
                return true;
            }
            if(!strcmp(pVariable->name,"z")) {
                z = 3;
                // delay declaration need to specify type.
                // variable type need while compiling script.
                pVariable->type = DataType::Integer;
                // delay updation need to specify data.
                // variable data need while evaluating script.
                pVariable->dataPtr = &z;
                return true;
            }
            return false;
        });

        scopedContext.setCustomScript(
            L"int foo(int a, int b) {\n"
            L"  return a + b + z;\n"
            L"}"
        );


        const wchar_t* expStr = L"foo(x, y)";
        ExpressionCpp e(expStr);

        setVariableUpdater(&e, [&y](Variable* pVariable){
            if(!strcmp(pVariable->name,"y")) {
                y = 2;
                // delay declaration need to specify type.
                // variable type need while compiling script.
                pVariable->type = DataType::Integer;
                // delay updation need to specify data.
                // variable data need while evaluating script.
                pVariable->dataPtr = &y;
                return true;
            }
            return false;
        });

        // delay update will run before expression is evaluated
        e.evaluate();
        ...
   }
```
# Build project
The project mainly use CMake to generated project files for native compilers such as Visual Studio, XCode, make file.  
In order to compile the main projects use following commands:  
```
mkdir build
cd build
cmake .. -DSKIP_UNIT_TEST_BUILD -DFFSCRIPT_EXCLUDE_THREAD
cmake --build . --config Release
```

In case you need to compile unit test project, you also need 'conan' installed on your system and folow below section to download gtest for the project.   
## Build steps
1. Add dependencies remote repositories  
 You may need to add 'bincrafters' into conan remote repositories.  
```
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```
2. Create dependencies directory.  
change directory to 'lib/ffscript/gtest' and create 'conan' directory.

3. Install dependencies.  
This project uses multi-configurations on Windows, so use following commands.  
```
conan install .. -g cmake_multi -s arch=x86 -s build_type=Release
conan install .. -g cmake_multi -s arch=x86 -s build_type=Debug
conan install .. -g cmake_multi -s arch=x86_64 -s build_type=Release
conan install .. -g cmake_multi -s arch=x86_64 -s build_type=Debug
```

For Linux(test on Ubuntu 16.04).  
```
conan install .. --build missing -s compiler.libcxx=libstdc++11
```
4. Compile whole project.  
```
mkdir build
cd build
cmake .. -DFFSCRIPT_EXCLUDE_THREAD=ON
cmake --build . --config Release
```
