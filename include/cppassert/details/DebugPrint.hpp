#pragma once
#ifndef CPPASSERT_DEBUGPRINT_HPP
#define	CPPASSERT_DEBUGPRINT_HPP

namespace cppassert
{
namespace internal
{
/**
 * Prints formatted message to stderr and flushes stderr
 *
 */
void DebugPrint(const char *message);

} //internal
} //cppassert

#endif	/* CPPASSERT_DEBUGPRINT_HPP */

