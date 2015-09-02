#include <gtest/gtest.h>
#include <cppassert/details/AssertionMessage.hpp>
#include <cstdint>

TEST(AssertionMessageTest, booleanTrue)
{
    const std::string expectedMessage("true");
    cppassert::internal::AssertionMessage message;
    message<<true;

    EXPECT_EQ(expectedMessage, message.str());
}

TEST(AssertionMessageTest, booleanFalse)
{
    const std::string expectedMessage("false");
    cppassert::internal::AssertionMessage message;
    message<<false;

    EXPECT_EQ(expectedMessage, message.str());
}

TEST(AssertionMessageTest, formatStdNullPtrt)
{
    const std::string expectedMessage("(null)");
    cppassert::internal::AssertionMessage message;
    message<<nullptr;

    EXPECT_EQ(expectedMessage, message.str());
}

TEST(AssertionMessageTest, formatNullPtr)
{
    const std::string expectedMessage("(null)");
    const char *nullPtr = nullptr;
    cppassert::internal::AssertionMessage message;
    message<<nullPtr;

    EXPECT_EQ(expectedMessage, message.str());
}

TEST(AssertionMessageTest, formatInt)
{
    const std::string expectedMessage("32767 2147483647 9223372036854775807");

    std::int16_t int16Value = 32767;
    std::int32_t int32Value = 2147483647;
    std::int64_t int64Value = 9223372036854775807;

    cppassert::internal::AssertionMessage message;
    message<<int16Value<<" "<<int32Value<<" "<<int64Value;
    EXPECT_EQ(expectedMessage, message.str());
}

TEST(AssertionMessageTest, manipulator)
{
    const std::string expectedMessage("ffff");
    std::int32_t int32Value = 0xffff;
    cppassert::internal::AssertionMessage message;
    message<<std::hex<<int32Value;
    EXPECT_EQ(expectedMessage, message.str());
}

struct CustomOperatorType
{
    std::int32_t mIntMember = 0xff;
    std::string mStringMember = "Test";

};

std::ostream &operator<<(std::ostream &out, const CustomOperatorType &custom)
{
    out<<custom.mIntMember<<" "<<custom.mStringMember;
    return out;
}

TEST(AssertionMessageTest, customOperatorTest)
{
    const std::string expectedMessage("255 Test");
    CustomOperatorType test;
    cppassert::internal::AssertionMessage message;
    message<<test;
    EXPECT_EQ(expectedMessage, message.str());
}

TEST(AssertionMessageTest, emptyMessage)
{
    cppassert::internal::AssertionMessage message;
    EXPECT_TRUE(message.empty());
    std::string str = message.str();
    EXPECT_TRUE(str.empty());
}

TEST(AssertionMessageTest, stdManipulatorTest)
{
    std::uint32_t n = 20;
    const std::string expectedMessage("0x14\n");
    cppassert::internal::AssertionMessage message;
    message<<std::hex<<std::showbase<<n<<std::endl;
    EXPECT_EQ(expectedMessage, message.str());
}

TEST(AssertionMessageTest, pointerTest)
{
    void *ptr = (void *)(0x0000000a);
    const std::string expectedMessage((sizeof(void *)==8  ? "0x000000000000000a" :  "0x0000000a" ));
    cppassert::internal::AssertionMessage message;
    message<<ptr;
    EXPECT_EQ(expectedMessage, message.str());
}