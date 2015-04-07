#pragma once
#ifndef CPP_ASSERT_ASSERTIONMESSAGE_HPP
#define	CPP_ASSERT_ASSERTIONMESSAGE_HPP
#include <memory>
#include <sstream>
#include <cstddef>

namespace cppassert
{
namespace internal
{

/**
 *  Provides streaming operator to CPP_ASSERT_* macros
 */
class AssertionMessage
{
    AssertionMessage(const AssertionMessage &) = delete;
    AssertionMessage &operator=(const AssertionMessage &) = delete;
public:
    /**
     * Move constructor, needs to be implemented by hand
     * until Visual C++ compiler will support generation default
     * implementation.
     * @param   other   Object to be moved
     */
    AssertionMessage(AssertionMessage &&other)
    {
            stream_ = std::move(other.stream_);
    }

    /**
     * Move assignment operator, needs to be implemented by hand
     * until Visual C++ compiler will support generation default
     * implementation.
     * @param   other   Object to be moved
     */
    AssertionMessage &operator=(AssertionMessage &&other)
    {
            stream_ = std::move(other.stream_);
            return (*this);
    }

    AssertionMessage();

    /**
     * Streams boolean value
     * @param   value   value to be streamed as text
     * @return  Reference to (*this) to allow chain calls
     */
    AssertionMessage& operator<<(bool value)
    {
        return *this << (value ? "true" : "false");
    }

    /**
     * Streams a non-pointer value to this object.
     * @param val
     * @return Reference to (*this) to allow chain calls
     */
    template <typename T>
    inline AssertionMessage& operator<<(const T& val)
    {
        getStream() << val;
        return *this;
    }
    /**
     * Streams pointer to this object, null pointers
     * should be displayed as "(null)"
     *
     * @param   pointer     Pointer to be streamed
     * @return  Reference to (*this) to allow chain calls
     */
    template <typename T>
    inline AssertionMessage& operator<<(T* const& pointer)
    {
        if(pointer == nullptr)
        {
            (*this)<<nullptr;
        }
        else
        {
            getStream()<<pointer;
        }
        return *this;
    }

    /**
     * Type definition for std::ostream manipulators like @c std::endl
     */
    using StdIoManipulatorType = std::ostream& (*)(std::ostream&);
    /**
     *  @brief  Interface for manipulators.
     *
     *  Manipulators such as @c std::endl and @c std::hex use these
     *  functions in constructs like "std::cout << std::endl".  For more
     *  information, see the iomanip header.
     */
    AssertionMessage& operator<<(StdIoManipulatorType manipulator)
    {
      getStream()<<manipulator;
      return (*this);
    }
    /**
     * Streams C++11 nullptr type
     * @param   nullptr
     * @return  Reference to (*this) to allow chain calls
     */
    AssertionMessage &operator<<(std::nullptr_t)
    {
        getStream()<<"(null)";
        return (*this);
    }
    /**
     * Returns text streamed to this object as std::string
     * @return  All the text streamed so far as std::string
     */
    std::string str() const;

    /**
     * Tests whether object contains any content streamed
     *
     * @return  true if there is content available
     */
    inline bool empty() const
    {
        return static_cast<bool>(stream_.get()==nullptr);
    }

private:
    inline std::ostream &getStream()
    {
        if(stream_.get()==nullptr)
        {
            stream_.reset(new std::stringstream);
        }
        return (*stream_);
    }

    std::unique_ptr<std::stringstream> stream_;
};


} //internal

using AssertionMessage = internal::AssertionMessage;

} //asrt

#endif	/* CPP_ASSERT_ASSERTIONMESSAGE_HPP */

