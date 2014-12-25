# What is this?

CppAssert is a modern C++11 assert replacement library. It provides basic
primitives for defensive programming. The behavior and message format can be
customized through available extension points.

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

## Supported compilers

This library is supported on following compilers
- g++ >= 4.8
- clang >= 3.5
- Visual C++ Compiler November 2013 CTP

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
