#include <cstdio>
#include <cstdint>
#include <cppassert/details/Helpers.hpp>

namespace cppassert
{
namespace internal
{
/**
 * Prints formatted message to stderr and flushes stderr
 *
 */
void PrintMessageToStdErr(const char *message)
{
    std::int32_t result = std::fprintf(stderr, "%s" , message);
    CPP_ASSERT_MARK_UNUSED(result);
    result = std::fflush(stderr);
    CPP_ASSERT_MARK_UNUSED(result);

}

} //internal
} //cppassert

