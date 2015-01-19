#include <cppassert/CppAssert.hpp>
#include <cppassert/details/DebugPrint.hpp>
#include <cppassert/details/AssertionMessage.hpp>
#include <cppassert/details/Helpers.hpp>
#include <cppassert/AssertionFailure.hpp>
#include <cppassert/details/StackTrace.hpp>
#include <cstdlib>
#include <cstdio>

namespace cppassert
{
using AssertionMessage = internal::AssertionMessage;

namespace internal
{


void onAssertionFailureDefaultHandler(const AssertionFailure &assertion)
{
    const std::string errorAsStr
        = CppAssert::getInstance()->formatAssertionMessage(assertion);

    PrintMessageToStdErr(errorAsStr.c_str());

#if defined(WIN32)
    /*
     * We used to call DebugBreak() on Windows, but amazingly, it causes
     * the MSVS 2010 debugger not to be able to recover a call stack.
     */
    *((int *) NULL) = 0;
    exit(3);
#elif defined(__APPLE__)
    /*
     * On Mac OS X, Breakpad ignores signals. Only real Mach exceptions are
     * trapped.
     */
    *((int *) NULL) = 0;  /* To continue from here in GDB: "return" then "continue". */
    ::abort();  /* In case above statement gets nixed by the optimizer. */
#else
    std::abort(); /* To continue from here in GDB: "signal 0". */
#endif

}

std::string formatBoolFailureMessage(const char* expressionText,
                                    const char* actualPredicateValue,
                                    const char* expectedPredicateValue)
{
    AssertionMessage msg;
    msg << "Assertion failure value of: " << expressionText
        << "\n  Actual: " << actualPredicateValue;
    msg << "\nExpected: " << expectedPredicateValue;
    return msg.str();
}

std::string formatStreamedMessage(const std::string &message)
{
    AssertionMessage msg;
    msg<<'\n'<<message;
    return msg.str();
}

std::string formatPredicateFailureMessage(const char* predicate,
                                    const char* value1Text,
                                    const char* value2Text,
                                    const std::string &value1,
                                    const std::string &value2)
{
    AssertionMessage msg;
    msg << "Assertion failure value of: ( " << value1Text <<' '<<predicate<<' '
        << value2Text<<" )";
    msg << "\n  "<<value1Text<<" evaluated to: "<<value1;
    msg << "\n  "<<value2Text<<" evaluated to: "<<value2;
    return msg.str();
}

std::string formatAssertionMessage(const AssertionFailure &assertion)
{
    AssertionMessage error;
    error<<assertion.getSourceFileName()<<':'<<assertion.getSourceFileLine()
            <<": "<<assertion.getFunctionName()<<": ";
    error<<assertion.getMessage()<<std::endl;
    error<<assertion.getStackTrace()<<std::endl;
    return error.str();
}

std::string formatStatementFailureMessage(const char *statement)
{
    AssertionMessage msg;
    msg<< "Assertion failure: "<<statement;
    return msg.str();
}

std::string formatFrame(std::uint32_t frameNumber
                        , const void *address
                        , const char *symbol )
{
    AssertionMessage msg;
    msg<<frameNumber<<' '
        <<address<<' '
        <<symbol<<std::endl;
    return msg.str();
}
} //internal

CppAssert::CppAssert()
:assertionHandler_(internal::onAssertionFailureDefaultHandler)
{
    setDefaultFormatter();
}

CppAssert *CppAssert::getInstance()
{
    static CppAssert instance;
    return &instance;
}

void CppAssert::onAssertionFailure(const AssertionFailure &assertion)
{
    std::unique_lock<std::mutex> lock(assertionHandlerMutex_);
    assertionHandler_(assertion);
}

std::string CppAssert::getStackTraceExceptTop(std::uint32_t skip)
{
    auto frames = internal::StackTrace::getStackTrace();
    //skip current frame
    skip += 1;
    AssertionMessage msg;
    if(frames.size()>skip)
    {
        std::uint32_t frameNumber = skip;
        for(; frameNumber<frames.size(); ++frameNumber)
        {
            msg<<formatter_.formatFrame_(frameNumber-skip
                            , frames[frameNumber].getAddress()
                            , frames[frameNumber].getSymbol());
        }
    }
    return msg.str();
}

std::string CppAssert::formatBoolFailureMessage(const char* expressionText,
                                    const char* actualPredicateValue,
                                    const char* expectedPredicateValue)
{
    return formatter_.formatBoolFailure_(expressionText
                            , actualPredicateValue
                            , expectedPredicateValue);
}

std::string CppAssert::formatStreamedMessage(const std::string &message)
{
    return formatter_.formatStreamed_(message);
}

std::string CppAssert::formatPredicateFailureMessage(const char* predicate,
                                    const char* value1Text,
                                    const char* value2Text,
                                    const std::string &value1,
                                    const std::string &value2)
{
    return formatter_.formatPredicateFailure_(predicate
                        , value1Text
                        , value2Text
                        , value1
                        , value2);
}

std::string CppAssert::formatAssertionMessage(const AssertionFailure &assertion)
{
    return formatter_.formatAssertion_(assertion);
}

std::string CppAssert::formatStatementFailureMessage(const char *statement)
{
    return formatter_.formatStatementFailure_(statement);
}

void CppAssert::setAssertionHandler(AssertionHandlerType assertionHandler)
{
    if(assertionHandler)
    {
        assertionHandler_ = std::move(assertionHandler);
    }
}

void CppAssert::setDefaultHandler()
{
    assertionHandler_ = internal::onAssertionFailureDefaultHandler;
}

void CppAssert::setBooleanFailureFormatter(CppAssert::FormatBoolFailure
                                                    booleanFormatter)
{
    if(booleanFormatter)
    {
        formatter_.formatBoolFailure_ = booleanFormatter;
    }
}


void CppAssert::setPredicateFailureFormatter(CppAssert::FormatPredicateFailure
                                                    predicateFormatter)
{
    if(predicateFormatter)
    {
        formatter_.formatPredicateFailure_ = predicateFormatter;
    }
}

void CppAssert::setStatementFailureFormatter(CppAssert::FormatStatementFailure
                                                    statementFormatter)
{
    if(statementFormatter)
    {
        formatter_.formatStatementFailure_ = statementFormatter;
    }
}

void CppAssert::setStreamFormatter(CppAssert::FormatStreamed streamFormatter)
{
    if(streamFormatter)
    {
        formatter_.formatStreamed_ = streamFormatter;
    }
}

void CppAssert::setFrameFormatter(CppAssert::FormatFrame frameFormatter)
{
    if(frameFormatter)
    {
        formatter_.formatFrame_ = frameFormatter;
    }
}

void CppAssert::setAssertionFormatter(CppAssert::FormatAssertion
                                            assertionFormatter)
{
    if(assertionFormatter)
    {
        formatter_.formatAssertion_ = assertionFormatter;
    }
}

void CppAssert::setDefaultFormatter()
{
    formatter_.formatAssertion_ = internal::formatAssertionMessage;
    formatter_.formatBoolFailure_ = internal::formatBoolFailureMessage;
    formatter_.formatPredicateFailure_ = internal::formatPredicateFailureMessage;
    formatter_.formatStatementFailure_ = internal::formatStatementFailureMessage;
    formatter_.formatStreamed_ = internal::formatStreamedMessage;
    formatter_.formatFrame_ = internal::formatFrame;
}

} //asrt
