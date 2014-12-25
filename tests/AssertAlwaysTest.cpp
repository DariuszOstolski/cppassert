#include <gtest/gtest.h>
#include <cppassert/Assertion.hpp>

#ifndef _WIN32
TEST(AssertAlwaysTest, assertShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS(false)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");
}


TEST(AssertAlwaysTest, assertTrueShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_TRUE(false)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(AssertAlwaysTest, assertFalseShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_FALSE(true)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(AssertAlwaysTest, assertNeShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    const char *a = nullptr;
    const char *b = nullptr;
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_NE(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");
}

TEST(AssertAlwaysTest, assertEqShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    const char *a = nullptr;
    const char *b = "baba";
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_EQ(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(AssertAlwaysTest, assertGtShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 1;
    std::int32_t b = 2;
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_GT(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(AssertAlwaysTest, assertGeShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 1;
    std::int32_t b = 2;
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_GE(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(AssertAlwaysTest, assertLtShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 2;
    std::int32_t b = 1;
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_LT(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(AssertAlwaysTest, assertLeShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 2;
    std::int32_t b = 1;
    EXPECT_EXIT(
            CPP_ASSERT_ALWAYS_LE(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}
#endif /* _WIN32 */
