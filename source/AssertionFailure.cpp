#include <cppassert/AssertionFailure.hpp>
#include <cppassert/CppAssert.hpp>

namespace cppassert
{

AssertionFailure::AssertionFailure(std::uint32_t line
                    , const char *file
                    , const char *functionName
                    , std::string &&message)
:sourceFileLine_(line), sourceFileName_(file), functionName_(functionName)
{
    message_<<message;
}

void AssertionFailure::operator=(const AssertionMessage &message)
{
    if(!message.empty())
    {
        message_<<CppAssert::getInstance()->formatStreamedMessage(message.str());
    }
    stackTrace_ = CppAssert::getInstance()->getStackTraceExceptTop(1);
    onAssertionFailure();
}

void AssertionFailure::onAssertionFailure(const AssertionMessage &message)
{
    if(!message.empty())
    {
        message_<<CppAssert::getInstance()->formatStreamedMessage(message.str());
    }
    stackTrace_ = CppAssert::getInstance()->getStackTraceExceptTop(1);
    onAssertionFailure();
}

void AssertionFailure::onAssertionFailure() const
{

    CppAssert::getInstance()->onAssertionFailure((*this));
}

std::uint32_t AssertionFailure::getSourceFileLine() const
{
    return sourceFileLine_;
}

const char *AssertionFailure::getSourceFileName() const
{
    return sourceFileName_;
}

const char *AssertionFailure::getFunctionName() const
{
    return functionName_;
}

const std::string &AssertionFailure::getStackTrace() const
{
    return stackTrace_;
}

std::string AssertionFailure::getMessage() const
{
    return message_.str();
}

std::string AssertionFailure::toString() const
{
    return CppAssert::getInstance()->formatAssertionMessage((*this));
}

} //asrt


