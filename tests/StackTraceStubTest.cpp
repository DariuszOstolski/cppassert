#include <gtest/gtest.h>
#include <stdexcept>
#ifdef CPP_ASSERT_HAVE_BACKTRACE
#undef CPP_ASSERT_HAVE_BACKTRACE
#endif
#ifdef _WIN32
#undef _WIN32
#endif
#include "../source/details/StackTrace.cpp"



using namespace cppassert::internal;
TEST(StackTraceTest, constructor)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_EQ(0u, frames.size());
}

TEST(StackTraceTest, moveAssignment)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_EQ(0u, frames.size());
    StackTrace other = StackTrace::getStackTrace();
    EXPECT_EQ(0u, other.size());
    other = std::move(frames);
    EXPECT_EQ(0u, other.size());
}

TEST(StackTraceTest, moveOperator)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_EQ(0u, frames.size());
    StackTrace other(std::move(frames));
    EXPECT_EQ(0u, other.size());
}

TEST(StackTraceTest, atShouldThrowOnStub)
{
    StackTrace frames = StackTrace::getStackTrace();
    EXPECT_EQ(0u, frames.size());
    EXPECT_THROW(frames[0], std::out_of_range);
}


