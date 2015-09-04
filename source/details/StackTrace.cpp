#include <cppassert/details/StackTrace.hpp>

namespace cppassert
{
namespace internal
{


const void *StackTrace::StackFrame::getAddress() const
{
    return address_;
}

const char *StackTrace::StackFrame::getSymbol() const
{
    return symbol_;
}

StackTrace::StackFrame::StackFrame(const void *address, const char *symbol)
    :address_(address), symbol_(symbol)
{

}

} //internal
} //asrt

#ifdef CPP_ASSERT_HAVE_BACKTRACE
#include "StackTraceGnu-inl.cpp"
#elif _WIN32
#include "StackTraceWin-inl.cpp"
#else
#include "StackTraceStub-inl.cpp"
#endif

namespace cppassert
{
namespace internal
{

StackTrace::StackTrace()
:impl_(new StackTraceImpl())
{

}

StackTrace::StackTrace(StackTrace &&other)
:impl_(std::move(other.impl_))
{

}

StackTrace &StackTrace::operator=(StackTrace &&other)
{
    impl_ = std::move(other.impl_);
    return (*this);
}

StackTrace::~StackTrace()
{

}

StackTrace StackTrace::getStackTrace()
{
    StackTrace frames;
    frames.impl_->collect();
    return frames;
}

std::size_t StackTrace::size() const
{
    return impl_->size();
}

const StackTrace::StackFrame &StackTrace::operator[](std::size_t position) const
{
    return impl_->at(position);
}

} //internal
} //asrt
