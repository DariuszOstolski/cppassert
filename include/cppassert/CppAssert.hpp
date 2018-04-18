#pragma once
#ifndef CPP_ASSERT_CPPASSERT_HPP
#define	CPP_ASSERT_CPPASSERT_HPP
#include <cstdint>
#include <string>
#include <functional>
#include <mutex>
#include <cppassert/details/DebugPrint.hpp>
#include <cppassert/details/AssertionMessage.hpp>
#include <cppassert/details/Helpers.hpp>
#include <cppassert/details/StackTrace.hpp>
#include <cppassert/AssertionFailure.hpp>


namespace cppassert
{
class AssertionFailure;

namespace internal
{
    void onAssertionFailureDefaultHandler(const AssertionFailure &assertion);
}

struct DefaultFormatter
{
    std::string formatAssertionMessage(const AssertionFailure &assertion);
    std::string formatBoolFailureMessage(const char* expressionText,
                                         const char* actualPredicateValue,
                                         const char* expectedPredicateValue);
    /**
     * Formats messages produced by equality/inequality macros
     * (CPP_ASSERT_[EQ|NE|LE|LT|GE|GT]
     */
    std::string formatPredicateFailureMessage(const char* predicate,
                                              const char* value1Text,
                                              const char* value2Text,
                                              const std::string &value1,
                                              const std::string &value2);
    std::string formatStatementFailureMessage(const char *statement);
    std::string formatStreamedMessage(const std::string &message);
    std::string formatFrame(std::uint32_t frameNumber
                            , const void *address
                            , const char *symbol);
};

template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
class CppAssertT
{
public:
    /**
     * Invoke assertion handler
     *
     * @param   assertion   Assertion that failed
     */
    void onAssertionFailure(const AssertionFailure &assertion);

    /**
     * Return stack trace except top frames as std::string except
     * number of frames
     * @param   frames      Number of frames to be skipped
     * @return  Stack trace as std::string
     */
    std::string getStackTraceExceptTop(std::uint32_t frames);

    /**
     * Returns a message for a bool assertion failures i.e. CPP_ASSERT_{TRUE|FALSE}
     *
     * Default implementation provides following string:
     *
     * @code
        AssertionMessage msg;
        msg << "Assertion failure value of: " << expressionText
            << "\n  Actual: " << actualPredicateValue;
        msg << "\nExpected: " << expectedPredicateValue;
     * @endcode
     *
     * @param   expressionText          Expression that failed as text
     * @param   actualPredicateValue    Actual value that expression evaluated to
     *                                  as text
     * @param   expectedPredicateValue  Expected value that expression should
     *                                  evaluate to
     * @return  User readable message about failed boolean assertion
     */
    std::string formatBoolFailureMessage(const char* expressionText,
                                    const char* actualPredicateValue,
                                    const char* expectedPredicateValue);

    /**
     * Returns a message for a predicate assertion failures.
     * Default implementation provides following message:
     * @code

    AssertionMessage msg;
    msg << "Assertion failure value of: ( " << value1Text <<' '<<predicate<<' '
        << value2Text<<" )";
    msg << "\n  "<<value1Text<<" evaluated to: "<<value1;
    msg << "\n  "<<value2Text<<" evaluated to: "<<value2;

     * @endcode
     * @param   predicate       Predicate text i.e. (value1 predicate value2)
     * @param   value1AsText    value1 as text i.e. variable name etc
     * @param   value2AsText    value2 as text i.e.
     * @param   value1          value1 evaluated to string using operator<<
     * @param   value2          value1 evaluated to string using operator<<
     * @return  String that should be displayed to user
     */
    std::string formatPredicateFailureMessage(const char* predicate,
                                    const char* value1AsText,
                                    const char* value2AsText,
                                    const std::string &value1,
                                    const std::string &value2);


    /**
     * Returns failure as std::string
     * @param   failure     Assertion failure to be displayed
     * @return  User readable description of failed assertion
     */
    std::string formatAssertionMessage(const AssertionFailure &failure);

    /**
     * Returns user readable description of failed statement. Default
     * implementation is to return following:
     * @code
        std::stringstream msg;
        msg<< "Assertion failure: "<<statement;
     * @endcode
     *
     * @param   statement   Statement that failed
     * @return  std::string with user readable description
     */
    std::string formatStatementFailureMessage(const char *statement);

    /**
     * This is used to format user streamed message to CPP_ASSERT macros
     * i.e. `CPP_ASSERT(false)<<"This shouldnt happen";`
     *
     * @param   message     User streamed message
     * @return User readable description of user custom message
     */
    std::string formatStreamedMessage(const std::string &message);
private:
    Formatter formatter_;
    LockingPolicy lockingPolicy_;
    AssertionHandler assertionHandler_;
};

struct DefaultHandler {
    void operator()(const AssertionFailure &assertion) {
        internal::onAssertionFailureDefaultHandler(assertion);
    }
};

using DefaultImplType = CppAssertT<DefaultFormatter, std::mutex, DefaultHandler>;
/**
 * Provides default functionality for CPP_ASSERT macros
 */
template<typename Impl>
class CppAssertI: public Impl
{
public:
    /**
     * Return a CppAssert object
     *
     * @return
     */
    static CppAssertI *getInstance()
    {
        static CppAssertI instance;
        return &instance;
    }

    /**
     * Invoke assertion handler
     *
     * @param   assertion   Assertion that failed
     */
    void onAssertionFailure(const AssertionFailure &assertion)
    {
        static_cast<Impl*>(this)->onAssertionFailure(assertion);
    }

    /**
     * Return stack trace except top frames as std::string except
     * number of frames
     * @param   frames      Number of frames to be skipped
     * @return  Stack trace as std::string
     */
    std::string getStackTraceExceptTop(std::uint32_t frames)
    {
        return static_cast<Impl*>(this)->getStackTraceExceptTop(frames);
    }


    /**
     * Returns a message for a bool assertion failures i.e. CPP_ASSERT_{TRUE|FALSE}
     *
     * Default implementation provides following string:
     *
     * @code
        AssertionMessage msg;
        msg << "Assertion failure value of: " << expressionText
            << "\n  Actual: " << actualPredicateValue;
        msg << "\nExpected: " << expectedPredicateValue;
     * @endcode
     *
     * @param   expressionText          Expression that failed as text
     * @param   actualPredicateValue    Actual value that expression evaluated to
     *                                  as text
     * @param   expectedPredicateValue  Expected value that expression should
     *                                  evaluate to
     * @return  User readable message about failed boolean assertion
     */
    std::string formatBoolFailureMessage(const char* expressionText,
                                    const char* actualPredicateValue,
                                    const char* expectedPredicateValue)
    {
        return static_cast<Impl*>(this)->formatBoolFailureMessage(expressionText,
                                                                  actualPredicateValue,
                                                                  expectedPredicateValue);
    }

    /**
     * Returns a message for a predicate assertion failures.
     * Default implementation provides following message:
     * @code

    AssertionMessage msg;
    msg << "Assertion failure value of: ( " << value1Text <<' '<<predicate<<' '
        << value2Text<<" )";
    msg << "\n  "<<value1Text<<" evaluated to: "<<value1;
    msg << "\n  "<<value2Text<<" evaluated to: "<<value2;

     * @endcode
     * @param   predicate       Predicate text i.e. (value1 predicate value2)
     * @param   value1AsText    value1 as text i.e. variable name etc
     * @param   value2AsText    value2 as text i.e.
     * @param   value1          value1 evaluated to string using operator<<
     * @param   value2          value1 evaluated to string using operator<<
     * @return  String that should be displayed to user
     */
    std::string formatPredicateFailureMessage(const char* predicate,
                                    const char* value1AsText,
                                    const char* value2AsText,
                                    const std::string &value1,
                                    const std::string &value2)
    {
        return static_cast<Impl*>(this)->formatPredicateFailureMessage(predicate,
                                                                       value1AsText,
                                                                       value2AsText,
                                                                       value1,
                                                                       value2);
    }


    /**
     * Returns failure as std::string
     * @param   failure     Assertion failure to be displayed
     * @return  User readable description of failed assertion
     */
    std::string formatAssertionMessage(const AssertionFailure &failure)
    {
        return static_cast<Impl*>(this)->formatAssertionMessage(failure);
    }

    /**
     * Returns user readable description of failed statement. Default
     * implementation is to return following:
     * @code
        std::stringstream msg;
        msg<< "Assertion failure: "<<statement;
     * @endcode
     *
     * @param   statement   Statement that failed
     * @return  std::string with user readable description
     */
    std::string formatStatementFailureMessage(const char *statement)
    {
        return static_cast<Impl*>(this)->formatStatementFailureMessage(statement);
    }

    /**
     * This is used to format user streamed message to CPP_ASSERT macros
     * i.e. `CPP_ASSERT(false)<<"This shouldnt happen";`
     *
     * @param   message     User streamed message
     * @return User readable description of user custom message
     */
    std::string formatStreamedMessage(const std::string &message)
    {
        return static_cast<Impl*>(this)->formatStreamedMessage(message);
    }
private:
    CppAssertI()
    {

    }

};

using CppAssert = CppAssertI<DefaultImplType>;

template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
void CppAssertT<Formatter, LockingPolicy, AssertionHandler>::onAssertionFailure(const AssertionFailure &assertion)
{
    AssertionHandler assertionHandler;
    {
        std::unique_lock<std::mutex> lock(lockingPolicy_);
        assertionHandler = assertionHandler_;
    }
    assertionHandler(assertion);
}

template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
std::string CppAssertT<Formatter, LockingPolicy, AssertionHandler>::getStackTraceExceptTop(std::uint32_t skip)
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
            msg<<formatter_.formatFrame(frameNumber-skip
                            , frames[frameNumber].getAddress()
                            , frames[frameNumber].getSymbol());
        }
    }
    return msg.str();
}

template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
std::string CppAssertT<Formatter, LockingPolicy, AssertionHandler>::formatBoolFailureMessage(const char* expressionText,
                                const char* actualPredicateValue,
                                const char* expectedPredicateValue)
{
    return formatter_.formatBoolFailureMessage(expressionText
                            , actualPredicateValue
                            , expectedPredicateValue);
}

template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
std::string CppAssertT<Formatter, LockingPolicy, AssertionHandler>::formatPredicateFailureMessage(const char* predicate,
                                const char* value1AsText,
                                const char* value2AsText,
                                const std::string &value1,
                                const std::string &value2)
{
    return formatter_.formatPredicateFailureMessage(predicate
                        , value1AsText
                        , value2AsText
                        , value1
                        , value2);
}


template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
std::string CppAssertT<Formatter, LockingPolicy, AssertionHandler>::formatAssertionMessage(const AssertionFailure &assertion)
{
    return formatter_.formatAssertionMessage(assertion);
}

template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
std::string CppAssertT<Formatter, LockingPolicy, AssertionHandler>::formatStatementFailureMessage(const char *statement)
{
    return formatter_.formatStatementFailureMessage(statement);
}

template<typename Formatter, typename LockingPolicy, typename AssertionHandler>
std::string CppAssertT<Formatter, LockingPolicy, AssertionHandler>::formatStreamedMessage(const std::string &message)
{
    return formatter_.formatStreamedMessage(message);
}

} //cppassert

#endif	/* CPP_ASSERT_CPPASSERT_HPP */

