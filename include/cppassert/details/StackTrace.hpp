#pragma once
#ifndef CPP_ASSERT_STACKTRACE_HPP
#define	CPP_ASSERT_STACKTRACE_HPP
#include <vector>
#include <memory>
#include <cstdint>

namespace cppassert
{
namespace internal
{

class StackTraceImpl;

/**
 * Portable wrapper for stack trace collection
 */
class StackTrace
{
public:

    class StackFrame
    {
        friend class StackTraceImpl;
    public:
        /**
         * Returns frame address or nullptr
         * if not available
         * @return
         */
        const void *getAddress() const;
        /**
         * Returns string that describe the address
         * symbolically
         *
         * @return symbol name or nu
         */
        const char *getSymbol() const;
    protected:
        StackFrame(const void *address, const char *symbol);
        StackFrame() = default;
        const void *address_ = nullptr;
        const char *symbol_ = nullptr;
    };

    /**
     * Move constructor
     * @param   other   stack trace to be moved
     */
    StackTrace(StackTrace &&other);

    /**
     * Moved assignment operator
     * @param   other   Stack trace to be moved
     * @return  Reference to this
     */
    StackTrace &operator=(StackTrace &&other);

    ~StackTrace();

    /**
     * Collects and returns current stack trace
     * @return  Stack trace
     */
    static StackTrace getStackTrace();

    /**
     * Returns number of frames
     * @return Number of frames returned
     */
    std::size_t size() const;

    /**
     * Returns stack frame at \p position, if position is greater
     * that size std::out_of_range exception is thrown
     * @param   position    Number of frame to be returned
     * @return StackFrame at \p position
     */
    const StackFrame &operator[](std::int32_t position) const;
private:
    StackTrace();
    StackTrace(const StackTrace &) = delete;
    StackTrace &operator=(const StackTrace &) = delete;

    std::unique_ptr<StackTraceImpl> impl_;
};
} //internal
} //asrt


#endif	/* CPP_ASSERT_STACKTRACE_HPP */

