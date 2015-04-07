#pragma once
#ifndef CPP_ASSERT_ASSERTIONFAILURE_HPP
#define	CPP_ASSERT_ASSERTIONFAILURE_HPP
#include "details/AssertionMessage.hpp"
#include <cstdint>

namespace cppassert
{

/**
 * @class AssertionFailure
 *
 * A class for indicating an assertion failure. The AssertionFailure object
 * remembers a non-empty message that describes how it failed and
 * associated context information.
 */
class AssertionFailure
{
    AssertionFailure(const AssertionFailure &) = delete;
    AssertionFailure operator=(const AssertionFailure &) = delete;
public:

    /**
     * @brief Creates an assertion failure.
     *
     * Creates assertion failure object with all context information required.
     * Please note that AssertionFailure objects are streamable so library
     * client may stream some custom message
     * @code

       AssertionFailure failure;
       failure<<"Some message";

     * @endcode
     *
     * @param[in]   line        Source file line where assertion failed
     * @param[in]   file        Source file name where assertion failed
     * @param[in]   function    Function name where assertion failed
     * @param[in]   message     Message associated with failed assertion
     *
     */
    AssertionFailure(std::uint32_t line
                    , const char *file
                    , const char *function
                    , std::string &&message);

    /**
     * Move constructor, have to be implemented by hand
     * because Visual C++ doesn't support generation of default ones
     *
     * @param   other   Object to be moved
     */
    AssertionFailure(AssertionFailure &&other)
    {
            sourceFileLine_ = other.sourceFileLine_;
            sourceFileName_ = other.sourceFileName_;
            functionName_ = other.functionName_;
            message_ = std::move(other.message_);
            stackTrace_ = std::move(other.stackTrace_);
            other.sourceFileLine_ = 0;
            other.sourceFileName_ = nullptr;
            other.functionName_ = nullptr;
    }

    /**
     * Move assignment operator, have to be implemented by hand
     * because Visual C++ doesn't support generation of default ones
     *
     * @param   other   Object to be moved
     */
    AssertionFailure &operator=(AssertionFailure &&other)
    {
            sourceFileLine_ = other.sourceFileLine_;
            sourceFileName_ = other.sourceFileName_;
            functionName_ = other.functionName_;
            message_ = std::move(other.message_);
            stackTrace_ = std::move(other.stackTrace_);
            other.sourceFileLine_ = 0;
            other.sourceFileName_ = nullptr;
            other.functionName_ = nullptr;
            return (*this);
    }


    /**
     * Std io manipulator type such as std::endl.
     */
    using StdIoManipulatorType = AssertionMessage::StdIoManipulatorType;

    /**
     *  @brief  Interface for manipulators.
     *
     *  Manipulators such as @c std::endl and @c std::hex use these
     *  functions in constructs like "std::cout << std::endl".  For more
     *  information, see the iomanip header.
     */
    AssertionFailure& operator<<(StdIoManipulatorType manipulator)
    {
        message_ << manipulator;
        return (*this);
    }

    /**
     * Streams value into assertion
     * @param[in]   value   Object that we want to have textual representation
     *                      in a message
     * @return Reference to this
     */
    template <typename T>
    AssertionFailure& operator<<(const T& value)
    {
        message_ << value;
        return *this;
    }

    /**
     * Returns source file line associated with failed assertion
     * @return source file line
     */
    std::uint32_t getSourceFileLine() const;

    /**
     * Returns source file name associated with failed assertion
     * @return source file name
     */
    const char *getSourceFileName() const;

    /**
     * Returns function name associated with failed assertion
     * @return function name
     * @note Value returned depends on compiler and is not portable
     *       some compilers may return mangled name
     */
    const char *getFunctionName() const;

    /**
     * Returns stack trace associated with a failed assertion
     * @return  Stack trace
     * @note    Please note that stack trace may not be available in
     *          some specific build configurations, especially when
     *          `-fomit-frame-pointer` option is used in your build
     */
    const std::string &getStackTrace() const;

    /**
     * Returns message associated with failed assertion
     * @return message associated with assertion
     */
    std::string getMessage() const;

    /**
     * Returns assertion as string formatted by installed assertion message
     * formatter. Message format depends on CPP_ASSERT_*() macro used to
     * create assertion. By
     * @return assertion failure represented as string
     */
    std::string toString() const;

    void onAssertionFailure(const AssertionMessage &message);
private:
    std::uint32_t sourceFileLine_ = 0;
    const char *sourceFileName_ = nullptr;
    const char *functionName_ = nullptr;
    AssertionMessage message_;
    std::string stackTrace_;
};

} //asrt

#endif	/* CPP_ASSERT_ASSERTIONFAILURE_HPP */

