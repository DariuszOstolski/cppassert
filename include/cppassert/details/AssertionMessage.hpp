#pragma once
#ifndef CPP_ASSERT_ASSERTIONMESSAGE_HPP
#define	CPP_ASSERT_ASSERTIONMESSAGE_HPP
#include <memory>
#include <sstream>
#include <cstddef>
#include <iomanip>
#include <type_traits>
#include <cstdint>

namespace cppassert
{
namespace internal
{

    template<typename T>
    struct PointerHelper 
    {
        PointerHelper(std::ostream &str)
            :stream_(str)
        {
            
        }
        
        inline std::ostream& write(const T *const &pointer) 
        {
             stream_<<pointer;
             return stream_;
        }
               
        
        std::ostream& stream_;
    };
    
    template<>
    struct PointerHelper<void>
    {
        PointerHelper(std::ostream &str)
            :stream_(str)
        {
            
        }
        
        std::ostream& write(void const *pointer) 
        {
            std::uintptr_t value = reinterpret_cast<std::uintptr_t>(pointer);
            stream_<<"0x"
                <<std::setfill ('0') << std::setw(sizeof(void const *)*2)
                <<std::hex
                <<value;
                         
            return stream_;
        }
                
        std::ostream& stream_;
    };
        
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
           using remove_cv_t = typename std::remove_cv<T>::type;
           PointerHelper<remove_cv_t>(getStream()).write(pointer);
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
        
        return  *stream_;
    }
   
    std::unique_ptr<std::stringstream> stream_;
};


} //internal

using AssertionMessage = internal::AssertionMessage;

} //asrt

#endif	/* CPP_ASSERT_ASSERTIONMESSAGE_HPP */

