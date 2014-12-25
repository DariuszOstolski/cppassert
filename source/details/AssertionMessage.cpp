#include "cppassert/details/AssertionMessage.hpp"

namespace cppassert
{
namespace internal
{

AssertionMessage::AssertionMessage()
{
}

std::string AssertionMessage::str() const
{
    if(stream_)
    {
        return stream_->str();
    }
    return std::string();
}


} //internal
} //asrt




