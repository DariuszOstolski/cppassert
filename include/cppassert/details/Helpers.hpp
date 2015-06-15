#pragma once
#ifndef CPP_ASSERT_HELPERS_HPP
#define	CPP_ASSERT_HELPERS_HPP
#include "AssertionMessage.hpp"

#define CPP_ASSERT_CONCAT(FIRST_TOKEN, SECOND_TOKEN) \
 CPP_ASSERT_CONCAT_IMPL(FIRST_TOKEN, SECOND_TOKEN)
#define CPP_ASSERT_CONCAT_IMPL(FIRST_TOKEN, SECOND_TOKEN) \
 FIRST_TOKEN ## SECOND_TOKEN

#define CPP_ASSERT_STRING(x)	#x

#define CPP_ASSERT_RETURN_COUNT(_1, _2, _3, count, ...) count
#define CPP_ASSERT_COUNT_ARGS_IMPL(args) \
   CPP_ASSERT_RETURN_COUNT args
#define CPP_ASSERT_COUNT_ARGS(...) \
   CPP_ASSERT_COUNT_ARGS_IMPL((__VA_ARGS__, 3, 2, 1, 0))

 /* Pick the right helper macro to invoke. */
#define CPP_ASSERT_OVERLOAD_MACRO2(name, count) name##count
#define CPP_ASSERT_OVERLOAD_MACRO1(name, count) CPP_ASSERT_OVERLOAD_MACRO2(name, count)
#define CPP_ASSERT_OVERLOAD_MACRO(name, count) CPP_ASSERT_OVERLOAD_MACRO1(name, count)

 /* The actual macro. */
#define CPP_ASSERT_GLUE(x, y) x y
#define CPP_ASSERT_CALL_OVERLOAD(name, ...) \
	CPP_ASSERT_GLUE(CPP_ASSERT_OVERLOAD_MACRO(name, CPP_ASSERT_COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))

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

#ifdef WIN32
#  define CPP_ASSERT_WHILE_FALSE __pragma( warning(push) ) \
               __pragma( warning(disable:4127) ) \
               while( false ) \
               __pragma( warning(pop) )
#else
#  define CPP_ASSERT_WHILE_FALSE while( FALSE )
#endif

#define CPP_ASSERT_VOID_CAST static_cast<void>
#define CPP_ASSERT_MARK_UNUSED(variable) static_cast<void>(variable)

#ifndef NDEBUG
# define CPP_ASSERT_ENABLED 1
#endif


#ifndef CPP_ASSERT_DISABLE_ALL

# define CPP_ASSERT_BOOL_IMPL_0_(expression, text, actual, expected) \
    do \
    { \
        if((expression)==expected) \
        { \
            ; \
        }\
        else \
        { \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getBoolAssertionFailureMessage(text, \
                                            CPP_ASSERT_STRING(actual), \
                                            CPP_ASSERT_STRING(expected))).onAssertionFailure(::cppassert::AssertionMessage()); \
        } \
    } CPP_ASSERT_WHILE_FALSE

# define CPP_ASSERT_BOOL_IMPL_1_(expression, text, actual, expected, message) \
    do \
    {   \
        if((expression)==expected) \
        { \
            ; \
        } \
        else \
        { \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getBoolAssertionFailureMessage(text, \
                                            CPP_ASSERT_STRING(actual), \
                                            CPP_ASSERT_STRING(expected))).onAssertionFailure(::cppassert::AssertionMessage()<<message); \
        } \
    } CPP_ASSERT_WHILE_FALSE

# define CPP_ASSERT_IMPL_0_(statement) \
    do \
    {   \
        if(statement) \
        {   \
            ; \
        } \
        else \
        { \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getAssertionFailureMessage( \
                                        CPP_ASSERT_STRING(statement) \
                                        ) ).onAssertionFailure(::cppassert::AssertionMessage()); \
        } \
    } CPP_ASSERT_WHILE_FALSE

# define CPP_ASSERT_IMPL_1_(statement, message) \
    do                  \
    {                   \
        if(statement)   \
        {               \
            ;           \
        }               \
        else            \
        {               \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getAssertionFailureMessage( \
                                        CPP_ASSERT_STRING(statement) \
                                        ) ).onAssertionFailure(::cppassert::AssertionMessage()<<message); \
        } \
    } CPP_ASSERT_WHILE_FALSE


# define CPP_ASSERT_PRED_IMPL_0_(val1, val2, val1Text, val2Text, predicate) \
    do \
    {   \
        if((val1) predicate (val2)) \
        {   \
            ; \
        }\
        else \
        {    ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getPredicateAssertionFailureMessage( \
                                        CPP_ASSERT_STRING(predicate), \
                                        val1Text, \
                                        val2Text, \
                                        std::move(::cppassert::AssertionMessage()<<val1), \
                                        std::move(::cppassert::AssertionMessage()<<val2))).onAssertionFailure(::cppassert::AssertionMessage()); \
        } \
    } CPP_ASSERT_WHILE_FALSE


# define CPP_ASSERT_PRED_IMPL_1_(val1, val2, val1Text, val2Text, predicate, message) \
    do \
    {   \
        if((val1) predicate (val2)) \
        { \
            ; \
        } \
        else \
        { \
            ::cppassert::AssertionFailure(__LINE__, __FILE__, \
                                    CPP_ASSERT_FUNCTION_NAME, \
                ::cppassert::internal::getPredicateAssertionFailureMessage( \
                                        CPP_ASSERT_STRING(predicate), \
                                        val1Text, \
                                        val2Text, \
                                        std::move(::cppassert::AssertionMessage()<<val1), \
                                        std::move(::cppassert::AssertionMessage()<<val2)) \
                                        ).onAssertionFailure(::cppassert::AssertionMessage()<<message); \
        } \
    } CPP_ASSERT_WHILE_FALSE



#else

# define CPP_ASSERT_IMPL_0_(statement)

# define CPP_ASSERT_IMPL_1_(statement, message)

# define CPP_ASSERT_BOOL_IMPL_0_(expression, text, actual, expected)

# define CPP_ASSERT_BOOL_IMPL_1_(expression, text, actual, expected, message)

# define CPP_ASSERT_PRED_IMPL_0_(val1, val2, val1Text, val2Text, predicate)

# define CPP_ASSERT_PRED_IMPL_1_(val1, val2, val1Text, val2Text, predicate, message)


#endif


#ifdef CPP_ASSERT_ENABLED

# define CPP_ASSERT_IMPL_1(statement) \
    CPP_ASSERT_IMPL_0_(statement)

# define CPP_ASSERT_IMPL_2(statement, message) \
    CPP_ASSERT_IMPL_1_(statement, message)

# define CPP_ASSERT_BOOL_IMPL_0(expression, text, actual, expected) \
    CPP_ASSERT_BOOL_IMPL_0_(expression, text, actual, expected)

# define CPP_ASSERT_BOOL_IMPL_1(expression, text, actual, expected, message) \
    CPP_ASSERT_BOOL_IMPL_1_(expression, text, actual, expected, message)

# define CPP_ASSERT_PRED_IMPL_0(val1, val2, val1Text, val2Text, predicate) \
    CPP_ASSERT_PRED_IMPL_0_(val1, val2, val1Text, val2Text, predicate)

# define CPP_ASSERT_PRED_IMPL_1(val1, val2, val1Text, val2Text, predicate, message) \
    CPP_ASSERT_PRED_IMPL_1_(val1, val2, val1Text, val2Text, predicate, message)

#else

# define CPP_ASSERT_IMPL_1(statement)

# define CPP_ASSERT_IMPL_2(statement, message)

# define CPP_ASSERT_BOOL_IMPL_0(expression, text, actual, expected)

# define CPP_ASSERT_BOOL_IMPL_1(expression, text, actual, expected, message)

# define CPP_ASSERT_PRED_IMPL_0(val1, val2, val1Text, val2Text, predicate)

# define CPP_ASSERT_PRED_IMPL_1(val1, val2, val1Text, val2Text, predicate, message)

#endif


#define CPP_ASSERT_ALWAYS_IMPL_1(statement) \
    CPP_ASSERT_IMPL_0_(statement)

#define CPP_ASSERT_ALWAYS_IMPL_2(statement, message) \
    CPP_ASSERT_IMPL_1_(statement, message)

#define CPP_ASSERT_ALWAYS_BOOL_0(expression, text, actual, expected) \
    CPP_ASSERT_BOOL_IMPL_0_(expression, text, actual, expected)

#define CPP_ASSERT_ALWAYS_BOOL_1(expression, text, actual, expected, message) \
    CPP_ASSERT_BOOL_IMPL_1_(expression, text, actual, expected, message)

#define CPP_ASSERT_ALWAYS_PRED_IMPL_0(val1, val2, val1Text, val2Text, predicate) \
   CPP_ASSERT_PRED_IMPL_0_(val1, val2, val1Text, val2Text, predicate)

#define CPP_ASSERT_ALWAYS_PRED_IMPL_1(val1, val2, val1Text, val2Text, predicate, message) \
   CPP_ASSERT_PRED_IMPL_1_(val1, val2, val1Text, val2Text, predicate, message)

#endif	/* CPP_ASSERT_HELPERS_HPP */

