cmake_minimum_required(VERSION 3.16)

################################
# Unit Tests
################################
# files containing the actual tests
set(test_sources
    DefaultAssertionHandlerTest.cpp
    AssertionMessageTest.cpp
    AssertionTest.cpp
    AssertionFailureTest.cpp
    CppAssertTest.cpp
    AssertAlwaysTest.cpp
)
add_executable( unitTests ${test_sources})
# Link test executable against gtest & gtest_main
find_package(GTest CONFIG REQUIRED)
target_link_libraries(unitTests PRIVATE GTest::gtest GTest::gtest_main cppassert)
add_test(unitTests unitTests)


set(test_sources
    StackTraceTest.cpp
)

add_executable( stackTraceTest StackTraceTest.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(stackTraceTest PRIVATE GTest::gtest GTest::gtest_main cppassert)
add_test(stackTraceTest stackTraceTest)

set(test_sources
    StackTraceStubTest.cpp
)

add_executable( stackTraceStubTest ${test_sources} )
# Link test executable against gtest & gtest_main
target_link_libraries(stackTraceStubTest PRIVATE GTest::gtest GTest::gtest_main cppassert)
add_test(stackTraceStubTest stackTraceStubTest  )