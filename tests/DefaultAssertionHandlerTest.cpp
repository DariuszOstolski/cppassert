#include <gtest/gtest.h>
#include <cppassert/Assertion.hpp>

//gtest doesnt support death tests on free bsd/windows
#if defined(__linux__) &&  !defined(NDEBUG)
TEST(DefaultAssertionHandlerTest, assertShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(
            CPP_ASSERT(false)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");
}


TEST(DefaultAssertionHandlerTest, assertTrueShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(
            CPP_ASSERT_TRUE(false)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(DefaultAssertionHandlerTest, assertFalseShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    EXPECT_EXIT(
            CPP_ASSERT_FALSE(true)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(DefaultAssertionHandlerTest, assertNeShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    const char *a = nullptr;
    const char *b = nullptr;
    EXPECT_EXIT(
            CPP_ASSERT_NE(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");
}

TEST(DefaultAssertionHandlerTest, assertEqShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    const char *a = nullptr;
    const char *b = "baba";
    EXPECT_EXIT(
            CPP_ASSERT_EQ(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(DefaultAssertionHandlerTest, assertGtShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 1;
    std::int32_t b = 2;
    EXPECT_EXIT(
            CPP_ASSERT_GT(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(DefaultAssertionHandlerTest, assertGeShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 1;
    std::int32_t b = 2;
    EXPECT_EXIT(
            CPP_ASSERT_GE(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(DefaultAssertionHandlerTest, assertLtShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 2;
    std::int32_t b = 1;
    EXPECT_EXIT(
            CPP_ASSERT_LT(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

TEST(DefaultAssertionHandlerTest, assertLeShouldAbort)
{
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    std::int32_t a = 2;
    std::int32_t b = 1;
    EXPECT_EXIT(
            CPP_ASSERT_LE(a, b)
            ,::testing::KilledBySignal(SIGABRT)
            , ".*Assertion failure.*");

}

#endif /* defined(__linux__) */
