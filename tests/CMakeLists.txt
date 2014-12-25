cmake_minimum_required(VERSION 2.8)


################################
# GTest
################################

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

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
set(EXECUTABLE_NAME unitTests)
add_executable( ${EXECUTABLE_NAME} ${test_sources} )
# Link test executable against gtest & gtest_main
target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main ${CPPASSERT_LIBNAME}  ${CPP_ASSERT_REQURED_LIBS})
add_test(unitTests ${EXECUTABLE_NAME})


set(test_sources
    StackTraceTest.cpp
)

set(EXECUTABLE_NAME stackTraceTest)
add_executable( ${EXECUTABLE_NAME} ${test_sources} )
# Link test executable against gtest & gtest_main
target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main ${CPP_ASSERT_REQURED_LIBS})
add_test(stackTraceTest ${EXECUTABLE_NAME})

set(test_sources
    StackTraceStubTest.cpp
)

set(EXECUTABLE_NAME stackTraceStubTest)
add_executable( ${EXECUTABLE_NAME} ${test_sources} )
# Link test executable against gtest & gtest_main
target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main ${CPP_ASSERT_REQURED_LIBS} )
add_test(stackTraceStubTest ${EXECUTABLE_NAME}  )