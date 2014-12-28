#include <cppassert/CppAssert.hpp>
#include <cppassert/AssertionFailure.hpp>
#include <gtest/gtest.h>
#include <functional>
#include <tuple>

class CppAssertTest : public ::testing::Test
{
protected:

    std::string formatBoolFailureMessage(const char* ,
                                    const char* ,
                                    const char* )
    {
        formatBoolCounter_ +=1;
        return std::string();
    }

    std::string formatStreamedMessage(const std::string &)
    {
        formatStreamedCounter_ +=1;
        return std::string();
    }

    std::string formatPredicateFailureMessage(const char* ,
                                        const char* ,
                                        const char* ,
                                        const std::string &,
                                        const std::string &)
    {
        formatPredicateCounter_ +=1;
        return std::string();
    }

    std::string formatAssertionMessage(const cppassert::AssertionFailure &)
    {
        formatAssertionCounter_ +=1;
        return std::string();
    }

    std::string formatStatementFailureMessage(const char *)
    {
        formatStatementCounter_ +=1;
        return std::string();
    }

    std::string formatFrame(std::uint32_t
                        , const void *
                        , const char *)
    {
        formatFrameCounter_ +=1;
        return std::string();
    }

    std::int32_t formatBoolCounter_ = 0;
    std::int32_t formatStreamedCounter_ = 0;
    std::int32_t formatPredicateCounter_ = 0;
    std::int32_t formatAssertionCounter_ = 0;
    std::int32_t formatStatementCounter_ = 0;
    std::int32_t formatFrameCounter_ = 0;

    cppassert::CppAssert *cppAssert_ = nullptr;
    virtual void SetUp()
    {
        formatBoolCounter_ = 0;
        formatStreamedCounter_ = 0;
        formatPredicateCounter_ = 0;
        formatAssertionCounter_ = 0;
        formatStatementCounter_ = 0;
        formatFrameCounter_ = 0;

        cppAssert_ = cppassert::CppAssert::getInstance();

        cppassert::CppAssert::Formatter formatter;
        formatter.formatAssertion_
                = std::bind<std::string>(&CppAssertTest::formatAssertionMessage
                            , this
                            , std::placeholders::_1
                            );

         formatter.formatBoolFailure_
                = std::bind<std::string>(&CppAssertTest::formatBoolFailureMessage
                            , this
                            , std::placeholders::_1
                            , std::placeholders::_2
                            , std::placeholders::_3);
        formatter.formatPredicateFailure_
                = std::bind<std::string>(&CppAssertTest::formatPredicateFailureMessage
                            , this
                            , std::placeholders::_1
                            , std::placeholders::_2
                            , std::placeholders::_3
                            , std::placeholders::_4
                            , std::placeholders::_5);
        formatter.formatStatementFailure_
                = std::bind<std::string>(&CppAssertTest::formatStatementFailureMessage
                            , this
                            , std::placeholders::_1);
        formatter.formatStreamed_
                = std::bind<std::string>(&CppAssertTest::formatStreamedMessage
                            , this
                            , std::placeholders::_1);
        formatter.formatFrame_
                = std::bind<std::string>(&CppAssertTest::formatFrame
                            , this
                            , std::placeholders::_1
                            , std::placeholders::_2
                            , std::placeholders::_3);

        cppAssert_->setFormatter(formatter);
    }

    virtual void TearDown()
    {
        formatBoolCounter_ = 0;
        formatStreamedCounter_ = 0;
        formatPredicateCounter_ = 0;
        formatAssertionCounter_ = 0;
        formatStatementCounter_ = 0;
        formatFrameCounter_ = 0;
    }
};


TEST_F(CppAssertTest, formatAssertion)
{
    cppassert::AssertionFailure asrt(0, nullptr, nullptr, std::string());
    cppAssert_->formatAssertionMessage(asrt);
    EXPECT_EQ(1, formatAssertionCounter_);
}

TEST_F(CppAssertTest, formatBoolAssertion)
{
    cppAssert_->formatBoolFailureMessage(nullptr, nullptr, nullptr);
    EXPECT_EQ(1, formatBoolCounter_);
}

TEST_F(CppAssertTest, formatPredicateAssertion)
{
    cppAssert_->formatPredicateFailureMessage(nullptr
                        , nullptr
                        , nullptr
                        , std::move(std::string())
                        , std::move(std::string()));
    EXPECT_EQ(1, formatPredicateCounter_);
}

TEST_F(CppAssertTest, formatStatementAssertion)
{
    cppAssert_->formatStatementFailureMessage(nullptr);
    EXPECT_EQ(1, formatStatementCounter_);
}

TEST_F(CppAssertTest, formatStreamed)
{
    cppAssert_->formatStreamedMessage(std::move(std::string()));
    EXPECT_EQ(1, formatStreamedCounter_);
}

TEST_F(CppAssertTest, formatFrame)
{
    std::string result = cppAssert_->getStackTraceExceptTop(0);
    if(result.empty())
    {
        EXPECT_EQ(0, formatFrameCounter_);
    }
    else
    {
        EXPECT_LT(1, formatFrameCounter_);
    }

}
