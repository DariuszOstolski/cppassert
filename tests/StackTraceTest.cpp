#include <gtest/gtest.h>
#include "../source/details/StackTrace.cpp"

#if defined(CPP_ASSERT_HAVE_BACKTRACE) || defined(_WIN32)

using namespace cppassert::internal;
TEST(StackTraceTest, constructor)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_TRUE(frames.size()>0);
}

TEST(StackTraceTest, moveAssignment)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_TRUE(frames.size()>0);
    StackTrace other = StackTrace::getStackTrace();
    EXPECT_TRUE(other.size()>0);
    other = std::move(frames);
    EXPECT_TRUE(other.size()>0);
}

TEST(StackTraceTest, moveOperator)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_TRUE(frames.size()>0);
    StackTrace other(std::move(frames));
    EXPECT_TRUE(other.size()>0);
}

TEST(StackTraceTest, getStackTrace)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_TRUE(frames.size()>0);
    const std::string functionName(frames[0].getSymbol());
    EXPECT_TRUE(functionName.find("getStackTrace")!=std::string::npos);
}

TEST(StackTraceTest, indexOperatorShouldThrowIfOutOfRange)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_TRUE(frames.size()>0);
    EXPECT_THROW(frames[frames.size()], std::out_of_range);
}
#endif /* defined(CPP_ASSERT_HAVE_BACKTRACE) || defined(_WIN32) */

#if defined(__linux__) || defined(__FreeBSD__)
TEST(StackTraceTest, demangler)
{
    const char *expectedSymbol =
#if defined(__linux__)    
        "void testing::internal::HandleSehExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*, void (testing::Test::*)(), char const*)";
#elif defined(__FreeBSD__)    
        "void testing::internal::HandleSehExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*, testing::internal::HandleSehExceptionsInMethodIfSupported<testing::Test, void>(void::*)(void), char const*)";
#endif     
    CppDemangler demangler;
    const char *demangledSymbol = demangler.demangle("_ZN7testing8internal38HandleSehExceptionsInMethodIfSupportedINS_4TestEvEET0_PT_MS4_FS3_vEPKc");
    EXPECT_STREQ(expectedSymbol, demangledSymbol);
}
#endif /* defined(__linux__) || defined(__FreeBSD__)*/
