#pragma once
#ifndef CPP_ASSERT_HELPERS_HPP
#define	CPP_ASSERT_HELPERS_HPP
#include "AssertionMessage.hpp"

#define CPP_ASSERT_CONCAT(FIRST_TOKEN, SECOND_TOKEN) \
 CPP_ASSERT_CONCAT_IMPL(FIRST_TOKEN, SECOND_TOKEN)
#define CPP_ASSERT_CONCAT_IMPL(FIRST_TOKEN, SECOND_TOKEN) \
 FIRST_TOKEN ## SECOND_TOKEN

#define CPP_ASSERT_STRING(x)	#x

namespace cppassert
{
namespace internal
{
/**
 * Returns a message for a bool assertion failures i.e. ASSERT_{TRUE|FALSE}
 *
 * @param expressionText
 * @param actualPredicateValue
 * @param expectedPredicateValue
 * @return
 */
std::string getBoolAssertionFailureMessage(
    const char* expressionText,
    const char* actualPredicateValue,
    const char* expectedPredicateValue);
/**
 * Returns a message for a predicate assertion failures
 *
 * @param   predicate       Predicate text i.e. (value1 predicate value2)
 * @param   value1AsText    value1 as text i.e. variable name etc
 * @param   value2AsText    value2 as text i.e.
 * @param   value1          value1 evaluated to string using operator<<
 * @param   value2          value1 evaluated to string using operator<<
 * @return  String that should be displayed to user
 */
std::string getPredicateAssertionFailureMessage(
    const char* predicate,
    const char* value1AsText,
    const char* value2AsText,
    AssertionMessage &&value1,
    AssertionMessage &&value2);

/**
 * Returns a message tor `CPP_ASSERT[_ALWAYS]` failures
 * @param   statement   Statement that failed
 * @return  String that should contain user readable message that statement failed
 */
std::string getAssertionFailureMessage(const char *statement);
} //internal
} //asrt

#if defined __GNUC__ ||   __clang__
#   define CPP_ASSERT_FUNCTION_NAME	__PRETTY_FUNCTION__
#elif defined ( WIN32 )
#	define CPP_ASSERT_FUNCTION_NAME __FUNCTION__
#else
#   define CPP_ASSERT_FUNCTION_NAME	__func__
#endif


#define CPP_ASSERT_VOID_CAST static_cast<void>
#define CPP_ASSERT_MARK_UNUSED(variable) static_cast<void>(variable)

#ifndef NDEBUG
# define CPP_ASSERT_ENABLED 1
#endif

// The GNU compiler emits a warning if nested "if" statements are followed by
// an "else" statement and braces are not used to explicitly disambiguate the
// "else" binding.  This leads to problems with code like:
//
//   if (gate)
//     ASSERT_*(condition) << "Some message";
//
// The "switch (0) case 0:" idiom is used to suppress this.
#ifdef __INTEL_COMPILER
# define CPP_ASSERT_AMBIGUOUS_ELSE_BLOCKER
#else
# define CPP_ASSERT_AMBIGUOUS_ELSE_BLOCKER switch (0) case 0: default:
#endif

#ifndef CPP_ASSERT_DISABLE_ALL
# define CPP_ASSERT_BOOL_IMPL_(expression, text, actual, expected) \
    CPP_ASSERT_AMBIGUOUS_ELSE_BLOCKER \
        if((expression)==expected) \
            ; \
        else \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getBoolAssertionFailureMessage(text, \
                                            CPP_ASSERT_STRING(actual), \
                                            CPP_ASSERT_STRING(expected))) = \
                                        ::cppassert::AssertionMessage()

# define CPP_ASSERT_IMPL_(statement) \
    CPP_ASSERT_AMBIGUOUS_ELSE_BLOCKER \
        if(statement) \
            ; \
        else \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getAssertionFailureMessage( \
                                        CPP_ASSERT_STRING(statement) \
                                        ) ) = \
                                        ::cppassert::AssertionMessage()

# define CPP_ASSERT_PRED_IMPL_(val1, val2, val1Text, val2Text, predicate) \
    CPP_ASSERT_AMBIGUOUS_ELSE_BLOCKER \
        if((val1) predicate (val2)) \
            ; \
        else \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getPredicateAssertionFailureMessage( \
                                        CPP_ASSERT_STRING(predicate), \
                                        val1Text, \
                                        val2Text, \
                                        std::move(::cppassert::AssertionMessage()<<val1), \
                                        std::move(::cppassert::AssertionMessage()<<val2))) = \
                                        ::cppassert::AssertionMessage()

#else
# define CPP_ASSERT_BOOL_IMPL_(expression, text, actual, expected) \
    ::cppassert::ReleaseAssertionMessage()

# define CPP_ASSERT_IMPL_(statement) \
    ::cppassert::ReleaseAssertionMessage()

# define CPP_ASSERT_PRED_IMPL_(val1, val2, val1Text, val2Text, predicate) \
    ::cppassert::ReleaseAssertionMessage()

#endif

#ifdef CPP_ASSERT_ENABLED
#define CPP_ASSERT_BOOL(expression, text, actual, expected) \
    CPP_ASSERT_BOOL_IMPL_(expression, text, actual, expected)

#define CPP_ASSERT_IMPL(statement) \
    CPP_ASSERT_IMPL_(statement)

#define CPP_ASSERT_PRED_IMPL(val1, val2, val1Text, val2Text, predicate) \
   CPP_ASSERT_PRED_IMPL_(val1, val2, val1Text, val2Text, predicate)

#else
# define CPP_ASSERT_BOOL(expression, text, actual, expected) \
        ::cppassert::ReleaseAssertionMessage()

# define CPP_ASSERT_PRED_IMPL(val1, val2, val1Text, val2Text, predicate)  \
        ::cppassert::ReleaseAssertionMessage()

# define CPP_ASSERT_IMPL(statement) \
        ::cppassert::ReleaseAssertionMessage()
#endif


#define CPP_ASSERT_ALWAYS_IMPL(statement) \
    CPP_ASSERT_IMPL_(statement)

#define CPP_ASSERT_ALWAYS_BOOL(expression, text, actual, expected) \
    CPP_ASSERT_BOOL_IMPL_(expression, text, actual, expected)

#define CPP_ASSERT_ALWAYS_PRED_IMPL(val1, val2, val1Text, val2Text, predicate) \
   CPP_ASSERT_PRED_IMPL_(val1, val2, val1Text, val2Text, predicate)

#endif	/* CPP_ASSERT_HELPERS_HPP */

