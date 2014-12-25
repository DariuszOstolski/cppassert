#include <cppassert/details/StackTrace.hpp>
#include <stdexcept>

namespace cppassert
{
namespace internal
{

/**
 * Provides empty implementation of stack trace collection
 * on platforms where this is not available.
 */
class StackTraceImpl
{
    StackTraceImpl(const StackTraceImpl &) = delete;
    StackTraceImpl &operator=(const StackTraceImpl &) = delete;
public:
    StackTraceImpl()
    {
    }

    ~StackTraceImpl()
    {
    }

    /**
     * Empty method for platforms where stack trace collecting
     * is not available
     */
    void collect()
    {
    }

    /**
     * Always returns 0
     * @return 0
     */
    std::size_t size() const
    {
        return 0;
    }

    /**
     * Throws `std::out_of_range` exception
     * 
     */
    const StackTrace::StackFrame &at(std::int32_t ) const
    {

        throw std::out_of_range("StackTrace[] out of range");
        //make compiler happy
        //return frames_[0];
    }

private:
    StackTrace::StackFrame frames_[1];
};



} //internal
} //asrt

