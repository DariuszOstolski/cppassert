set(srcs
    details/AssertionMessage.cpp
    details/Helpers.cpp
    details/StackTrace.cpp
    Assertion.cpp
    AssertionFailure.cpp
    CppAssert.cpp

)

set(target_name ${CPPASSERT_LIBNAME})

include_directories(${CPP_ASSERT_REQURED_INCLUDE_DIRS})
add_library(${target_name} STATIC ${srcs})
target_link_libraries(${target_name} ${CPP_ASSERT_REQURED_LIBS})
