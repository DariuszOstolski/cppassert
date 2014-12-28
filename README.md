# What is this?

CppAssert is a modern C++11 assert replacement library. It provides "assert-like" 
macros: `CPP_ASSERT_??()`, `CPP_ASSERT_ALWAYS_??()` which can be used to 
enable optional *redundant* runtime checks in corresponding build modes. 
If an assertion argument evaluates to false, a runtime-configurable assertion
handler is invoked with the context information about current filename,
line number, expression text and with optionally evaluated arguments.
Default assertion handler provides detailed information about failed assertion
can easily customize messages as well as handler behavior using provided
and a corresponding failure location including stack trace.  Library clients
can easily customize messages as well as handler behavior using provided
extension points.

Assertion failure is a sign of a contract violation or some other logic
error. The goal of assertion failure is to report precise location and
nature of defect clearly and loudly. Assertions are enabled during compile
time, when enabled, each of the macros provided in CppAssert does the same
thing: each macro tests the predicate expression 'X', and, if (X) evaluates
to false, invokes the currently installed handler function having the signature

    void AssertionHandler(const AssertionFailure &assertion);

passing AssertionFailure object. Note that if an assertions are disabled
it expands to nothing by the preprocessor. Default assertion handler displays
detailed message about assertion location including stack trace and evaluated
arguments.

# Where are things?

`build/` is where code is built - like where executables are.
`3rdparty/` includes 3rdparty libraries, currently only gtest-1.7.0 is required.
`doc/` contains doxygen documentation.
`include/` this is a public API of a library.
`source/` this directory contains source code for library.
`tests/` contains unit tests for a library.
`CMakeLists.txt` must be in each subdirectory of the project.

# What do I do?

## Getting Started

To start using CppAssert, you need to compile CppAssert into a 
library and link your project with it.  We provide cmake files 
that can be used to generate files for following tools: Borland 
Makefiles, MSYS Makefiles, MinGW Makefiles, NMake Makefiles, NMake
Makefiles JOM, Ninja, Unix Makefiles, Watcom WMake, Visual Studio
12 2013, Xcode. If your build system is not on this list, you can 
take a look at CMakeLists.txt to learn how CppAssert should be compiled
(basically you want to compile src/* with CPP_ASSERT_ROOT/include in the 
header search path, where CPP_ASSERT_ROOT is the CppAssert root directory).

Once you are able to compile the CppAssert library, you should create 
a project or build target for your project. Make sure you have CPP_ASSERT_ROOT/include 
in the header search path so that the compiler can find "cppassert/Assertion.hpp"
when compiling your project. Set up your project to link with the 
CppAssert library (for example, in Visual Studio, this is done by 
adding a dependency on cppassert.vcproj).

If you still have questions, take a look at how CppAssert tests are 
built and use them as examples. 

## Use cmake and make
If you want to test it all out through the common gtest procedure, first
**delete build/** (if present). Then...

In the project root:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE="Test" ../

By now Makefiles should be created.
Then, to build executables and do all that linking stuff,

    make

To run all tests easily,

    ctest

## Examples

```
#include <iostream>
#include <cppassert/Assertion.hpp>

using namespace std;
int check_args(int size)
{
    CPP_ASSERT_ALWAYS(size!=0)<<"Size shouldnt be 0";
    return 0;
}

int main(int argc, char **)
{
    const int ARGS_MAX = 5;
    std::cerr<<"Hello "<<argc<<std::endl;
    CPP_ASSERT_LT(argc, ARGS_MAX)<<" argc is not lower than "
                                <<ARGS_MAX<<std::endl;
    std::cerr<<"World "<<std::endl;
    check_args(0);
    return 0;
}

``` 
And corresponding output:

```
./cppassertExample 
Hello 1
World 
/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure: size!=0
Size shouldnt be 0
0 0x40ffe2 ./cppassertExample(check_args(int)+0xf5) [0x40ffe2]
1 0x41058b ./cppassertExample(main+0x472) [0x41058b]
2 0x7f8094557ec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f8094557ec5]
3 0x40fe29 ./cppassertExample() [0x40fe29]

Aborted (core dumped)
```

## Supported compilers

This library is supported on following compilers
- g++ >= 4.8
- clang >= 3.4.1
- Visual C++ Compiler >= 2012
- on Oracle Solaris Studio >= 12.4 will compile without test, currently there is 
  a bug(compiler assertion fails) in compiler for which Im waiting to be 
  resolved

## Supported operating systems

- Linux,
- Windows,
- FreeBSD,
- Solaris (gcc works out of box, for Oracle Solaris Studio compiler needs
  a fix for test build)


## Supported architectures

CppAssert is supported on following architectures:
- i686
- x86-64
- arm (using gcc)
- PowerPc (using gcc)

## Cross compilation

To cross compile for arm use provided toolchain file:

    cmake -DCMAKE_BUILD_TYPE=Test -DCMAKE_TOOLCHAIN_FILE=../cmake/Modules/Arm6.cmake ../

for arm based architectures, or

    cmake -DCMAKE_BUILD_TYPE=Test -DCMAKE_TOOLCHAIN_FILE=../cmake/Modules/PowerPc.cmake ../

for PowerPc.
