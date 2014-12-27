#pragma once
#ifndef CPP_ASSERT_CPPASSERT_HPP
#define	CPP_ASSERT_CPPASSERT_HPP
#include <cstdint>
#include <string>
#include <functional>
#include <mutex>

namespace cppassert
{
class AssertionFailure;

/**
 * Provides default functionality for CPP_ASSERT macros
 */
class CppAssert
{
public:

    /**
     * Callback type for assertion handler. It can be both
     * a function or a some class method(using std::bind).
     *
     */
    using AssertionHandlerType = std::function<void(const AssertionFailure &assertion)>;

    /**
     *  Provides types definitions for formating functions
     */
    struct Formatter
    {
        /**
         * Formats messages produced by CPP_ASSERT_[TRUE|FALSE] macros
         */
        using FormatBoolFailure
                = std::function<std::string(const char* expressionText,
                                    const char* actualPredicateValue,
                                    const char* expectedPredicateValue)>;
        /**
         * Formats messages produced by equality/inequality macros
         * (CPP_ASSERT_[EQ|NE|LE|LT|GE|GT]
         */
        using FormatPredicateFailure
                = std::function<std::string(const char* predicate,
                                    const char* value1Text,
                                    const char* value2Text,
                                    const std::string &value1,
                                    const std::string &value2)>;

        /**
         * Type for assertion message formatter
         */
        using FormatAssertion
                = std::function<std::string(const AssertionFailure &)>;

        /**
         * Formats message produced by CPP_ASSERT macro.
         */
        using FormatStatementFailure
                = std::function<std::string(const char *statement)>;

        /**
         * Formats message streamed by operator<<
         */
        using FormatStreamed
                = std::function<std::string(const std::string &message)>;

        /**
         * Formats stack frame
         */
        using FormatFrame
                = std::function<std::string(std::uint32_t frameNumber
                                    , const void *address
                                    , const char *symbol)>;

        /**
         * Callback used to format boolean assertion
         */
        FormatBoolFailure formatBoolFailure_;
        /**
         * Callback used to format assertion predicates
         */
        FormatPredicateFailure formatPredicateFailure_;
        /**
         * Callback used to format failed assertions
         */
        FormatAssertion formatAssertion_;
        /**
         * Callback used to format statement failures
         */
        FormatStatementFailure formatStatementFailure_;
        /**
         * Callback used to format streamed user messages
         */
        FormatStreamed formatStreamed_;
        /**
         * Callback used to format stack frames
         */
        FormatFrame formatFrame_;
    };

    /**
     * Return a CppAssert object
     *
     * @return
     */
    static CppAssert *getInstance();

    /**
     * Set user provided formatter object.
     *
     * @note Not all members of formatter structure have to be set. If they
     * are not set default implementation will be used
     *
     * @param[in]   formatter   object with
     *
     */
    void setFormatter(Formatter formatter);

    /**
     * Set user provided assertion handler.
     *
     * @param assertionHandler
     */
    void setAssertionHandler(AssertionHandlerType assertionHandler);

    /**
     * Sets default assertion handler
     */
    void setDefaultHandler();

    /**
     * Sets default formatting functions
     */
    void setDefaultFormatter();
    
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
    CppAssert();
    AssertionHandlerType assertionHandler_;
    Formatter formatter_;
    std::mutex assertionHandlerMutex_;

};

} //asrt

#endif	/* CPP_ASSERT_CPPASSERT_HPP */

