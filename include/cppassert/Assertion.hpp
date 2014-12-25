#pragma once
#ifndef CPP_ASSERT_ASSERTION_HPP
#define	CPP_ASSERT_ASSERTION_HPP
#include "details/Helpers.hpp"
#include "details/AssertionMessage.hpp"
#include "AssertionFailure.hpp"

/** @file */

/**
 * @mainpage CppAssert - build-specific, runtime-configurable assertions.
 *
 * @section Introduction
 *
 * CppAssert is a library that provides "assert-like" macros: 'CPP_ASSERT_??()',
 * 'CPP_ASSERT_ALWAYS_??()' which can be used to enable optional *redundant*
 * runtime checks in corresponding build modes. If an assertion argument
 * evaluates to false, a runtime-configurable assertion
 * handler is invoked with the context information about current filename,
 * line number, expression text and with optionally evaluated arguments.
 * Default assertion handler provides detailed information about failed assertion
 * and a corresponding failure location including stack trace. Library clients
 * can easily customize messages as well as handler behavior using provided
 * extension points.
 *
 * @section dprogramming Defensive programming
 *
 * According to Wikipedia: "Defensive programming is a form of defensive design
 * intended to ensure the continuing function of a piece of software under
 * unforeseen circumstances. The idea can be viewed as reducing or eliminating
 * the prospect of Finagle's law having effect. Defensive programming techniques
 * are used especially when a piece of software could be misused.
 *
 * Defensive programming is an approach to improve software and source code,
 * in terms of:
 *  - General quality - reducing the number of software bugs and problems.
 *  - Making the source code comprehensible - the source code should be
 *    readable and understandable so it is approved in a code audit.
 *  - Making the software behave in a predictable manner despite unexpected
 *    inputs or user actions."
 *
 * This library concentrates on the 3rd technique of defensive programming
 * making software behave in predictable manner despite unexpected inputs. Using
 * CppAssert library author component may provide optional runtime checks of
 * preconditions (or invariants) defined in the function-level documentation
 * (contract) for that component. It allows to expose defects early in the
 * development process. It should never be used to mask or recover from
 * them in production.
 *
 * Source:
 *   - http://en.wikipedia.org/wiki/Defensive_programming
 *   - http://en.wikipedia.org/wiki/Secure_input_and_output_handling
 *
 * There are three important aspects of assertions:
 *   - Every assertion is redundant
 *   - Each boolean-valued assert argument must have no side-effects
 *   - Assertions do not affect binary compatibility
 *
 *
 * @section Behavior
 *
 * Assertion failure is a sign of a contract violation or some other logic
 * error. The goal of assertion failure is to report precise location and
 * nature of defect clearly and loudly. Assertions are enabled during compile
 * time, when enabled, each of the macros provided in
 * CppAssert does the same thing: each macro tests the predicate expression 'X',
 * and, if (X) evaluates to false, invokes the currently installed
 * handler function having the signature
 *
 * @code

    void AssertionHandler(const AssertionFailure &assertion);

 * @endcode
 *
 * passing AssertionFailure object. Note that if an assertions are disabled
 * it expands to nothing by the preprocessor.
 *
 * @section compilation Compilation modes
 *
 * If NDEBUG macro was not defined all assertions are incorporated into
 * translation unit. If CPP_ASSERT_DISABLE_ALL macro was defined during
 * compile time all CPP_ASSERT_* macros are removed.
 *
 * @section customize Customization
 *
 * CppAssert library can be customized through available extension points.
 *
 * @subsection assertionHandler Assertion handler customization
 *
 * One of extension point allows library client to replace assertion handler
 * with a custom one. Assertion handler that conform to following signature:
 *
 * @code

    std::function<void(const AssertionFailure &assertion)

 * @endcode
 *
 * Assertion handler can be changed using CppAssert::setAssertionHandler, for
 * example:
 *
 * @code

    class MyApplication
    {
    public:
        void onAssertion(const cppassert::AssertionFailure &assertion)
        {
            //some code reporting assertion object
        }

        virtual void Init()
        {
            auto assertionHandler = std::bind(
                                    &AssertionTest::onAssertion
                                    , this
                                    , std::placeholders::_1);

            cppassert::CppAssert::getInstance()->setAssertionHandler(assertionHandler);
        }
    };
 *
 * @endcode
 *
 * @subsection formatter Assertion message formatting
 *
 * CppAssert library uses 6 types of formatting functions:
 *   - `std::function<std::string(const char* expressionText, const char* actualPredicateValue, const char* expectedPredicateValue)>`
 *      it is used format messages produced by CPP_ASSERT_[TRUE|FALSE] macros
 *   - `std::function<std::string(const char* predicate, const char* value1Text, const char* value2Text, const std::string &value1, const std::string &value2)>`
 *      it is used to format message with CPP_ASSERT_[EQ|NE|LE|LT|GE|GT] macros
 *   - `std::function<std::string(const AssertionFailure &)>`
 *      it is used to format assertion failure message by assertion handler
 *   - `std::function<std::string(const char *statement)>`
 *      it is used to format assertion failure message produced by CPP_ASSERT macro
 *   - `std::function<std::string(std::string &&message)>`
 *      it is used to format a message streamed to assertion failure using operator<<
 *   - `std::function<std::string(std::uint32_t frameNumber , const void *address , const char *symbol)>;`
 *      it is used to format single stack frame
 *
 * Every function should return std::string.
 *
 * To change given formatter to custom one cppassert::CppAssert::Formatter struct
 * can be used. Please note that You don't have to provide all formatter functions
 * it's completely valid to replace only one, where in that case default formatter
 * functions will be used.
 *
 * Example:
 * @code

class MyFormatter
{
public:

    std::string formatBoolFailureMessage(const char* expressionText
                                  , const char* actualPredicateValue
                                  , const char* expectedPredicateValue)
    {
      std::string result;
      //...some code
      return result;
    }

    std::string formatStreamedMessage(std::string &&message)
    {
      std::string result;
      //...some code
      return result;
    }

    std::string formatPredicateFailureMessage(const char* predicate
                                            , const char* value1Text
                                            , const char* value2Text
                                            , const std::string &value1
                                            , const std::string &value2)
    {
      std::string result;
      //...some code
      return result;
    }

    std::string formatAssertionMessage(const cppassert::AssertionFailure &failure)
    {
      std::string result;
      //...some code
      return result;
    }

    std::string formatStatementFailureMessage(const char *statement)
    {
      std::string result;
      //...some code
      return result;
    }

    std::string formatFrame(std::uint32_t frameNumber
                        , const void *frameAddress
                        , const char *frameSymbol)
    {
      std::string result;
      //...some code
      return result;
    }

    void initializeFormatter()
    {

        cppassert::CppAssert *cppAssert = cppassert::CppAssert::getInstance();

        cppassert::CppAssert::Formatter formatter;
        formatter.formatAssertion_
                = std::bind(&MyFormatter::formatAssertionMessage
                            , this
                            , std::placeholders::_1
                            );

        formatter.formatBoolFailure_
                = std::bind(&MyFormatter::formatBoolFailureMessage
                            , this
                            , std::placeholders::_1
                            , std::placeholders::_2
                            , std::placeholders::_3);
        formatter.formatPredicateFailure_
                = std::bind(&MyFormatter::formatPredicateFailureMessage
                            , this
                            , std::placeholders::_1
                            , std::placeholders::_2
                            , std::placeholders::_3
                            , std::placeholders::_4
                            , std::placeholders::_5);
        formatter.formatStatementFailure_
                = std::bind(&MyFormatter::formatStatementFailureMessage
                            , this
                            , std::placeholders::_1);
        formatter.formatStreamed_
                = std::bind(&MyFormatter::formatStreamedMessage
                            , this
                            , std::placeholders::_1);
        formatter.formatFrame_
                = std::bind(&MyFormatter::formatFrame
                            , this
                            , std::placeholders::_1
                            , std::placeholders::_2
                            , std::placeholders::_3);

        cppAssert->setFormatter(formatter);
    }
};

 * @endcode
 *
 */

/**
 * @defgroup CPP_ASSERT Debug build runtime assertions
 *
 * @brief Abort a program if assertion is false in debug mode builds.
 *
 * If  the  macro  NDEBUG was defined at the moment this header was last
 * included, `CPP_ASSERT_*` macros generate no code, and hence do nothing at all.
 *
 * If expression doesn't fulfill condition assertion handler is invoked, which
 * by default prints a message, call stack to standard error and terminates the
 * program by calling  abort.
 *
 * Library clients can stream custom message using operator<<
 *
 * Examples:
 *
 * @code

    CPP_ASSERT(array_size!=0) << "Array size is 0 ";
    CPP_ASSERT_TRUE(obj!=nullptr);
    CPP_ASSERT_FALSE(obj==nullptr);
    CPP_ASSERT_EQ(array, v2):
    CPP_ASSERT_NE(array, nullptr) << " array is null ";
    CPP_ASSERT_LT(i, array_size);
    CPP_ASSERT_GT(records.size(), 0) << "There is no record left.";
    CPP_ASSERT_LT(v1, v2) <<" v1 is not less than v2 ";
    CPP_ASSERT_LE(v1, v2) <<" v1 is not less than equal v2 ";
    CPP_ASSERT_GT(v1, v2) <<" v1 is not greater than v2 ";
    CPP_ASSERT_GE(v1, v2) <<" v1 is not greater equal than v2";

 * @endcode
 * @{
 */

/**
 * Verifies that statement evaluates to true. If not assertion handler
 * is invoked.
 *
 * Example:
 * @code

   CPP_ASSERT(size!=0)<<"Array size cannot be 0";

 * @endcode
 * And corresponding output
 * @code

/samples/cppassert.cpp:9: int check_args(int): Assertion failure: size!=0
Vector size cannot be 0
0 0x40fd32 ./samples/cppassertExample(check_args(int)+0xf5) [0x40fd32]
1 0x4102db ./samples/cppassertExample(main+0x472) [0x4102db]
2 0x7f15aaaeaec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f15aaaeaec5]
3 0x40fb79 ./samples/cppassertExample() [0x40fb79]

Aborted (core dumped)

 * @endcode
 */
#define CPP_ASSERT(statement) \
    CPP_ASSERT_IMPL(statement)

/**
 * Verifies that a boolean condition is true. If condition doesn't evaluate
 * to true assertion handler is invoked.
 * Example:
 *
 * @code

   CPP_ASSERT_TRUE(condition)<<"Condition should be true";

 * @endcode
 *
 * And corresponding output
 *
 * @code

/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure value of: condition
  Actual: false
Expected: true
Condition should be true
0 0x40fce6 ./samples/cppassertExample(check_args(int)+0x109) [0x40fce6]
1 0x41028f ./samples/cppassertExample(main+0x472) [0x41028f]
2 0x7f09e9643ec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f09e9643ec5]
3 0x40fb19 ./samples/cppassertExample() [0x40fb19]

Aborted (core dumped)

 * @endcode
 */
#define CPP_ASSERT_TRUE(condition) \
  CPP_ASSERT_BOOL(condition, #condition, false, true)

/**
 * Verifies that a boolean condition is false. If condition doesn't evaluate
 * to false assertion handler is invoked.
 * @code

   CPP_ASSERT_FALSE(size==0)<<"Size shouldnt be 0";

 * @endcode
 *
 * And corresponding output using default assertion handler
 *
 * @code

/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure value of: size==0
  Actual: true
Expected: false
Size shouldnt be 0
0 0x40fd3c ./samples/cppassertExample(check_args(int)+0xff) [0x40fd3c]
1 0x4102e5 ./samples/cppassertExample(main+0x472) [0x4102e5]
2 0x7feab041aec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7feab041aec5]
3 0x40fb79 ./samples/cppassertExample() [0x40fb79]

Aborted (core dumped)

 * @endcode
 */
#define CPP_ASSERT_FALSE(condition) \
  CPP_ASSERT_BOOL(condition, #condition, true, false)

/**
 * @{
 * Assertions for testing equalities and inequalities.
 *
 * @code

   CPP_ASSERT_EQ(v1, v2):           Tests that v1 == v2
   CPP_ASSERT_NE(v1, v2):           Tests that v1 != v2
   CPP_ASSERT_LT(v1, v2):           Tests that v1 < v2
   CPP_ASSERT_LE(v1, v2):           Tests that v1 <= v2
   CPP_ASSERT_GT(v1, v2):           Tests that v1 > v2
   CPP_ASSERT_GE(v1, v2):           Tests that v1 >= v2

 * @endcode
 *
 * When predicate evaluates to false CppAssert invokes assertion handler.
 *
 * Example:
 *
 * @code

   CPP_ASSERT_NE(size, 0)<<"Size shouldnt be 0";

 * @endcode
 *
 * And corresponding output:
 *
 * @code


/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure value of: ( size != 0 )
  size evaluated to: 0
  0 evaluated to: 0
Size shouldnt be 0
0 0x40fe3d ./samples/cppassertExample(check_args(int)+0x200) [0x40fe3d]
1 0x4104ab ./samples/cppassertExample(main+0x472) [0x4104ab]
2 0x7f82a794fec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f82a794fec5]
3 0x40fb79 ./samples/cppassertExample() [0x40fb79]

Aborted (core dumped)

 * @endcode
 *
 * Default assertion handler prints
 * - the tested expressions,
 * - their actual values,
 * - call stack
 * and terminates the program by calling abort.
 *
 * To make a user-defined type work with {CPP_ASSERT}_??(), it is requires to
 * overload comparison operators. If You can't overload
 * comparison operators You are advised to use {CPP_ASSERT}_{TRUE|FALSE}_()
 * macros to assert that 2 objects are equal.
 *
 * These macros evaluate their arguments exactly once.
 *
 */

#define CPP_ASSERT_EQ(val1, val2) \
  CPP_ASSERT_PRED_IMPL(val1, val2,  CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), ==)
#define CPP_ASSERT_NE(val1, val2) \
  CPP_ASSERT_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), !=)
#define CPP_ASSERT_LE(val1, val2) \
  CPP_ASSERT_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), <=)
#define CPP_ASSERT_LT(val1, val2) \
  CPP_ASSERT_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), <)
#define CPP_ASSERT_GE(val1, val2) \
  CPP_ASSERT_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), >=)
#define CPP_ASSERT_GT(val1, val2) \
  CPP_ASSERT_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), >)
/** @}*/

/**
 * @}
 */

/**
 * @defgroup CPP_ASSERT_ALWAYS Release build runtime assertions
 *
 * @brief Abort a program if assertion is false.
 *
 * Those macros generate code even in release builds. To disable them in release
 * mode builds define `CPP_ASSERT_DISABLE_ALL` at the moment this header was last
 * included.
 *
 * If expression doesn't fulfill condition assertion handler is invoked, which
 * by default prints a message, callstack to standard error and terminates the
 * program by calling  abort.
 *
 * @code

    CPP_ASSERT_ALWAYS(array_size!=0) << "Array size is 0 ";
    CPP_ASSERT_ALWAYS_TRUE(obj!=nullptr);
    CPP_ASSERT_ALWAYS_FALSE(obj==nullptr);
    CPP_ASSERT_ALWAYS_EQ(array, v2):
    CPP_ASSERT_ALWAYS_NE(array, nullptr) << " array is null ";
    CPP_ASSERT_ALWAYS_LT(i, array_size);
    CPP_ASSERT_ALWAYS_GT(records.size(), 0) << "There is no record left.";
    CPP_ASSERT_ALWAYS_LT(v1, v2) <<" v1 is not less than v2 ";
    CPP_ASSERT_ALWAYS_LE(v1, v2) <<" v1 is not less than equal v2 ";
    CPP_ASSERT_ALWAYS_GT(v1, v2) <<" v1 is not greater than v2 ";
    CPP_ASSERT_ALWAYS_GE(v1, v2) <<" v1 is not greater equal than v2";

 * @endcode
 *
 *
 * @{
 */

/**
 * Verifies that statement evaluates to true. If not assertion handler
 * is invoked.
 *
 *
 * Example:
 *
 * @code

   CPP_ASSERT_ALWAYS(size==0)<<"Size shouldnt be 0";

 * @endcode
 *
 * And corresponding output:
 *
 * @code

/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure: size!=0
Size shouldnt be 0
0 0x40fd32 ./samples/cppassertExample(check_args(int)+0xf5) [0x40fd32]
1 0x4102db ./samples/cppassertExample(main+0x472) [0x4102db]
2 0x7f33ebb51ec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f33ebb51ec5]
3 0x40fb79 ./samples/cppassertExample() [0x40fb79]

Aborted (core dumped)

 * @endcode
 */
#define CPP_ASSERT_ALWAYS(statement) \
    CPP_ASSERT_ALWAYS_IMPL(statement)

/**
 * Verifies that condition evaluates to true. If condition doesn't evaluate
 * to true assertion handler is invoked.
 *
 * @code

   CPP_ASSERT_ALWAYS_TRUE(condition)<<"Condition should be true";

 * @endcode
 *
 * And corresponding output
 *
 * @code

/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure value of: condition
  Actual: false
Expected: true
Condition should be true
0 0x40fce6 ./samples/cppassertExample(check_args(int)+0x109) [0x40fce6]
1 0x41028f ./samples/cppassertExample(main+0x472) [0x41028f]
2 0x7f09e9643ec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f09e9643ec5]
3 0x40fb19 ./samples/cppassertExample() [0x40fb19]

Aborted (core dumped)

 * @endcode
 */
#define CPP_ASSERT_ALWAYS_TRUE(condition) \
  CPP_ASSERT_ALWAYS_BOOL(condition, #condition, false, true)

/**
 * Verifies that a boolean condition is false. If condition doesn't evaluate
 * to false assertion handler is invoked.
 *
 * @code

   CPP_ASSERT_ALWAYS_FALSE(size==0)<<"Size shouldnt be 0";

 * @endcode
 *
 * And corresponding output using default assertion handler
 *
 * @code

/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure value of: size==0
  Actual: true
Expected: false
Size shouldnt be 0
0 0x40fd3c ./samples/cppassertExample(check_args(int)+0xff) [0x40fd3c]
1 0x4102e5 ./samples/cppassertExample(main+0x472) [0x4102e5]
2 0x7feab041aec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7feab041aec5]
3 0x40fb79 ./samples/cppassertExample() [0x40fb79]

Aborted (core dumped)

 * @endcode
 */
#define CPP_ASSERT_ALWAYS_FALSE(condition) \
  CPP_ASSERT_ALWAYS_BOOL(condition, #condition, true, false)

/**
 * @{
 * Assertions for testing equalities and inequalities.
 *
 * @code

  CPP_ASSERT_ALWAYS_EQ(v1, v2):           Tests that v1 == v2
  CPP_ASSERT_ALWAYS_NE(v1, v2):           Tests that v1 != v2
  CPP_ASSERT_ALWAYS_LT(v1, v2):           Tests that v1 < v2
  CPP_ASSERT_ALWAYS_LE(v1, v2):           Tests that v1 <= v2
  CPP_ASSERT_ALWAYS_GT(v1, v2):           Tests that v1 > v2
  CPP_ASSERT_ALWAYS_GE(v1, v2):           Tests that v1 >= v2

 * @endcode
 *
 * When they are not CppAssert invokes assertion handler.
 *
 * Default assertion handler prints
 * - the tested expressions,
 * - their actual values,
 * - call stack
 * and terminates the program by calling abort.
 *
 * To make a user-defined type work with {CPP_ASSERT}_??(), it is requires to
 * overload comparison operators. If You can't overload
 * comparison operators You are advised to use {CPP_ASSERT}_{TRUE|FALSE}_()
 * macros to assert that 2 objects are equal.
 *
 * These macros evaluate their arguments exactly once.
 *
 * Example:
 *
 * @code

   CPP_ASSERT_ALWAYS_NE(size, 0)<<"Size shouldnt be 0";

 * @endcode
 *
 * And corresponding output:
 *
 * @code


/cppassert/samples/cppassert.cpp:10: int check_args(int): Assertion failure value of: ( size != 0 )
  size evaluated to: 0
  0 evaluated to: 0
Size shouldnt be 0
0 0x40fe3d ./samples/cppassertExample(check_args(int)+0x200) [0x40fe3d]
1 0x4104ab ./samples/cppassertExample(main+0x472) [0x4104ab]
2 0x7f82a794fec5 /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f82a794fec5]
3 0x40fb79 ./samples/cppassertExample() [0x40fb79]

Aborted (core dumped)

 * @endcode
 */
#define CPP_ASSERT_ALWAYS_EQ(val1, val2) \
  CPP_ASSERT_ALWAYS_PRED_IMPL(val1, val2,  CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), ==)
#define CPP_ASSERT_ALWAYS_NE(val1, val2) \
  CPP_ASSERT_ALWAYS_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), !=)
#define CPP_ASSERT_ALWAYS_LE(val1, val2) \
  CPP_ASSERT_ALWAYS_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), <=)
#define CPP_ASSERT_ALWAYS_LT(val1, val2) \
  CPP_ASSERT_ALWAYS_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), <)
#define CPP_ASSERT_ALWAYS_GE(val1, val2) \
  CPP_ASSERT_ALWAYS_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), >=)
#define CPP_ASSERT_ALWAYS_GT(val1, val2) \
  CPP_ASSERT_ALWAYS_PRED_IMPL(val1, val2, CPP_ASSERT_STRING(val1), CPP_ASSERT_STRING(val2), >)
/** @}*/

/**
 * @}
 */


#endif	/* CPP_ASSERT_ASSERTION_HPP */

