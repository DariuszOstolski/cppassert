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
} //internal

std::string DefaultFormatter::formatBoolFailureMessage(const char* expressionText,
                                    const char* actualPredicateValue,
                                    const char* expectedPredicateValue)
{
    AssertionMessage msg;
    msg << "Assertion failure value of: " << expressionText
        << "\n  Actual: " << actualPredicateValue;
    msg << "\nExpected: " << expectedPredicateValue;
    return msg.str();
}

std::string DefaultFormatter::formatStreamedMessage(const std::string &message)
{
    AssertionMessage msg;
    msg<<'\n'<<message;
    return msg.str();
}

std::string DefaultFormatter::formatPredicateFailureMessage(const char* predicate,
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

std::string DefaultFormatter::formatAssertionMessage(const AssertionFailure &assertion)
{
    AssertionMessage error;
    error<<assertion.getSourceFileName()<<':'<<assertion.getSourceFileLine()
            <<": "<<assertion.getFunctionName()<<": ";
    error<<assertion.getMessage()<<std::endl;
    error<<assertion.getStackTrace()<<std::endl;
    return error.str();
}

std::string DefaultFormatter::formatStatementFailureMessage(const char *statement)
{
    AssertionMessage msg;
    msg<< "Assertion failure: "<<statement;
    return msg.str();
}

std::string DefaultFormatter::formatFrame(std::uint32_t frameNumber
                        , const void *address
                        , const char *symbol )
{
    AssertionMessage msg;
    msg<<std::setw(4)<<frameNumber<<' '
        <<address<<' '
        <<symbol<<std::endl;
    return msg.str();
}



} //asrt
