#include <cppassert/AssertionFailure.hpp>
#include <cppassert/CppAssert.hpp>
#include <gtest/gtest.h>

class AssertionFailureTest : public ::testing::Test
{
protected:

    

    

    virtual void SetUp()
    {    
       cppassert::CppAssert::getInstance()->setDefaultFormatter();
    }

};

TEST_F(AssertionFailureTest, accessors)
{
    const std::uint32_t expectedLine = 0xff;
    const char expectedFile[] = "file";
    const char expectedFunction[] = "my_function";
    const std::string expectedMessage("my message");

    std::string message(expectedMessage);

    cppassert::AssertionFailure assertion(expectedLine
                                , expectedFile
                                , expectedFunction
                                , std::move(message));

    EXPECT_EQ(expectedLine, assertion.getSourceFileLine());
    EXPECT_STREQ(expectedFile, assertion.getSourceFileName());
    EXPECT_STREQ(expectedFunction, assertion.getFunctionName());
    EXPECT_EQ(expectedMessage, assertion.getMessage());
    EXPECT_TRUE(assertion.getStackTrace().empty());
}

TEST_F(AssertionFailureTest, toString)
{
    const std::int32_t expectedLine = 0xff;
    const char expectedFile[] = "file";
    const char expectedFunction[] = "my_function";
    std::string message("my message");

    cppassert::AssertionFailure assertion(expectedLine
                                , expectedFile
                                , expectedFunction
                                , std::move(message));
    const std::string expectedMessage = R"(file:255: my_function: my message

)";
    EXPECT_EQ(expectedMessage, assertion.toString());
}

static void emptyHandler(const cppassert::AssertionFailure &)
{

}

TEST_F(AssertionFailureTest, operatorEq)
{
    const std::int32_t expectedLine = 0xff;
    const char expectedFile[] = "file";
    const char expectedFunction[] = "my_function";
    std::string message("my message");
    cppassert::CppAssert::getInstance()->setAssertionHandler(emptyHandler);

    cppassert::AssertionFailure assertion(expectedLine
                                , expectedFile
                                , expectedFunction
                                , std::move(message));
    cppassert::AssertionMessage msg;
    msg<<"test";
    assertion = msg;

    const std::string expectedMessage = R"(file:255: my_function: my message
test
)";
    EXPECT_TRUE(assertion.toString().find(expectedMessage)==0);
}
