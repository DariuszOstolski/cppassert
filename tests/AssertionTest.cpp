#include <gtest/gtest.h>
#include <cppassert/Assertion.hpp>
#include <cppassert/CppAssert.hpp>

#ifdef CPP_ASSERT_ENABLED
class AssertionTest : public ::testing::Test
{
protected:

    std::string formatStreamedMessage(const std::string &msg)
    {
        EXPECT_EQ(expectedStreamMessage_, msg);
        return "\n"+msg;
    }

    std::int32_t assertionHandlerCounter_ = 0;
    std::string  expectedStreamMessage_;

    void onAssertion(const cppassert::AssertionFailure &assertion)
    {
        std::cerr<<assertion.toString();
        assertionHandlerCounter_ += 1;
    }

    virtual void SetUp()
    {
       assertionHandlerCounter_ = 0;
       auto assertionHandler = std::bind<void>(
                                    &AssertionTest::onAssertion
                                    , this
                                    , std::placeholders::_1);

       auto formatStreamed_
                = std::bind<std::string>(&AssertionTest::formatStreamedMessage
                            , this
                            , std::placeholders::_1);

       //cppassert::CppAssert::getInstance()->setStreamFormatter(formatStreamed_);
       //cppassert::CppAssert::getInstance()->setAssertionHandler(assertionHandler);
    }

    virtual void TearDown()
    {
        //cppassert::CppAssert::getInstance()->setDefaultHandler();
    }
};


TEST_F(AssertionTest, assertHandlerShouldBeCalled)
{
    std::int32_t a = 0;
    std::int32_t b = 1;
    CPP_ASSERT(a==b);
    EXPECT_EQ(1, assertionHandlerCounter_);
    expectedStreamMessage_ = "a is not equal to b";
    CPP_ASSERT(a==b, expectedStreamMessage_);
    EXPECT_EQ(2, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertHandlerShouldNotBeCalled)
{
    std::int32_t a = 0;
    std::int32_t b = 1;
    CPP_ASSERT(a!=b);
    EXPECT_EQ(0, assertionHandlerCounter_);
    CPP_ASSERT(a!=b, "");
    EXPECT_EQ(0, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertTrueHandlerShouldBeCalled)
{
    bool condition = false;
    CPP_ASSERT_TRUE(condition);
    EXPECT_EQ(1, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertTrueHandlerShouldNotBeCalled)
{
    bool condition = true;
    CPP_ASSERT_TRUE(condition);
    EXPECT_EQ(0, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertFalseHandlerShouldBeCalled)
{
    bool condition = true;
    CPP_ASSERT_FALSE(condition);
    EXPECT_EQ(1, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertFalseHandlerShouldNotBeCalled)
{
    bool condition = false;
    CPP_ASSERT_FALSE(condition);
    EXPECT_EQ(0, assertionHandlerCounter_);
}

#define EXPECTED_TEST_MSG(a, b) "Expected message: "<<a<<" "<<b
#define SET_EXPECTED_MSG(a, b) \
    std::stringstream expectedMsg; \
    expectedMsg<<"Expected message: "<<a<<" "<<b; \
    expectedStreamMessage_ = expectedMsg.str() \


TEST_F(AssertionTest, assertEqHandlerShouldBeCalled)
{
    std::int32_t a = 0;
    std::int32_t b = 1;
    CPP_ASSERT_EQ(a, b);
    EXPECT_EQ(1, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);

    CPP_ASSERT_EQ(a, b, EXPECTED_TEST_MSG(a, b));
    EXPECT_EQ(2, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertEqHandlerShouldNotBeCalled)
{
    std::int32_t a = 1;
    std::int32_t b = 1;
    CPP_ASSERT_EQ(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_EQ(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(0, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertNeHandlerShouldBeCalled)
{
    std::int32_t a = 1;
    std::int32_t b = 1;
    CPP_ASSERT_NE(a, b);
    EXPECT_EQ(1, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_NE(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(2, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertNeHandlerShouldNotBeCalled)
{
    std::int32_t a = 0;
    std::int32_t b = 1;
    CPP_ASSERT_NE(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_NE(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(0, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertLeHandlerShouldBeCalled)
{
    std::int32_t a = 2;
    std::int32_t b = 1;
    CPP_ASSERT_LE(a, b);
    EXPECT_EQ(1, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_LE(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(2, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertLeHandlerShouldNotBeCalled)
{
    std::int32_t a = 1;
    std::int32_t b = 1;
    CPP_ASSERT_LE(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);
    a = 0;
    CPP_ASSERT_LE(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_LE(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(0, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertLtHandlerShouldBeCalled)
{
    std::int32_t a = 1;
    std::int32_t b = 1;
    CPP_ASSERT_LT(a, b);
    EXPECT_EQ(1, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_LT(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(2, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertLtHandlerShouldNotBeCalled)
{
    std::int32_t a = 0;
    std::int32_t b = 1;
    CPP_ASSERT_LT(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_LT(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(0, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertGeHandlerShouldBeCalled)
{
    std::int32_t a = 0;
    std::int32_t b = 1;
    CPP_ASSERT_GE(a, b);
    EXPECT_EQ(1, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_GE(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(2, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertGeHandlerShouldNotBeCalled)
{
    std::int32_t a = 1;
    std::int32_t b = 1;
    CPP_ASSERT_GE(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);
    a = 2;
    CPP_ASSERT_GE(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_GE(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(0, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertGtHandlerShouldBeCalled)
{
    std::int32_t a = 1;
    std::int32_t b = 1;
    CPP_ASSERT_GT(a, b);
    EXPECT_EQ(1, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_GT(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(2, assertionHandlerCounter_);
}

TEST_F(AssertionTest, assertGtHandlerShouldNotBeCalled)
{
    std::int32_t a = 2;
    std::int32_t b = 1;
    CPP_ASSERT_GT(a, b);
    EXPECT_EQ(0, assertionHandlerCounter_);

    SET_EXPECTED_MSG(a, b);
    CPP_ASSERT_GT(a, b, EXPECTED_TEST_MSG(a,b));
    EXPECT_EQ(0, assertionHandlerCounter_);
}

#endif /* CPP_ASSERT_ENABLED */
