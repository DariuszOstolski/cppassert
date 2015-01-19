#include <cppassert/details/DebugPrint.hpp>
#include <cppassert/details/StackTrace.hpp>
#include <stdexcept>
#include <memory>
#include <mutex>
#include <cstdlib>
#include <Windows.h>
#include <DbgHelp.h>
#include <strsafe.h>

namespace cppassert
{
    namespace internal
    {

        /**
         * Displays error message related to last occured error
         *
         */
        void DisplayLastError(LPTSTR lpszFunction)
        {
            // Retrieve the system error message for the last-error code

            LPVOID lpMsgBuf;
            LPVOID lpDisplayBuf;
            DWORD dw = GetLastError();

            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf,
                0, NULL);

            // Display the error message and exit the process

            lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));

            StringCchPrintf((LPTSTR)lpDisplayBuf,
                LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                TEXT("%s failed with error %d: %s"),
                lpszFunction, dw, lpMsgBuf);

            DebugPrint((LPCSTR)lpDisplayBuf);
            
            LocalFree(lpMsgBuf);
            LocalFree(lpDisplayBuf);
        }

        /**
        * Provides windows implementation of stack trace collection.
        */
        class StackTraceImpl
        {
            StackTraceImpl(const StackTraceImpl &) = delete;
            StackTraceImpl &operator=(const StackTraceImpl &) = delete;
            struct BacktraceSymbol : public StackTrace::StackFrame
            {
                BacktraceSymbol() = default;

                BacktraceSymbol(const void *frameAddr, const char *name)
                {
                    address_ = frameAddr;
                    setSymbol(name);
                }

                void setAddress(const void *frameAddress)
                {
                    address_ = frameAddress;
                }

                void setSymbol(const char *name)
                {
                    if (name && name[0])
                    {
                        strcpy_s(symbolName_, MaxSymbolNameSize, name);
                        symbolName_[MaxSymbolNameSize - 1] = '\0';
                    }
                    else
                    {
                        symbolName_[0] = '\0';
                    }
                    symbol_ = symbolName_;
                }

                enum { MaxSymbolNameSize = MAX_SYM_NAME + 1 };
                char symbolName_[MaxSymbolNameSize];
            };
        public:
            StackTraceImpl()
            {
            }

            ~StackTraceImpl()
            {
            }

            void collect()
            {
                HANDLE              process;
                ULONG               frame;
                PSYMBOL_INFO        symbol;
                DWORD64             displacement;
                PVOID               frames[cFramesSize];
                enum {              MaxNameLen = MAX_SYM_NAME + 1 };
                enum {              BufferSize = sizeof(SYMBOL_INFO)
                                                + MaxNameLen * sizeof(TCHAR) };

                char buffer[BufferSize] = { '\0' };

                symbol = static_cast<PSYMBOL_INFO>(static_cast<void *>(buffer));

                std::memset(symbol, 0, BufferSize);
                symbol->MaxNameLen = MaxNameLen-1;
                symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

                process = GetCurrentProcess();
                displacement = 0;
                std::unique_lock<std::mutex> lock(mutex_);

                if (SymInitialize(process, NULL, TRUE) != TRUE)
                {
                    DisplayLastError("SymInitialize");
                    return;
                }

                capturedFrames_ = CaptureStackBackTrace(cFramesToSkip
                                            , cFramesSize
                                            , frames
                                            , NULL);

                if (capturedFrames_ > cFramesSize)
                {
                    capturedFrames_ = cFramesSize;
                }

                for (frame = 0; frame<capturedFrames_; frame++)
                {
                    DWORD64 frameAddr = (DWORD64)(frames[frame]);
                    frames_[frame].setAddress(frames[frame]);
                    if (SymFromAddr(process, frameAddr, &displacement, symbol)
                                    == TRUE)
                    {
                        frames_[frame].setSymbol(symbol->Name);
                    }
                    else
                    {
                        DisplayLastError("SymFromAddr");
                    }
                }
            }

            /**
            * Returns number of captured frames
            * @return number of captured frames
            */
            std::size_t size() const
            {
                return capturedFrames_;
            }

            /**
             * Returns StackFrame at position \p pos. If pos
             * is outside range (0, size()) throws std::out_of_range exception.
             */
            const StackTrace::StackFrame &at(std::uint32_t pos) const
            {
                if (pos<capturedFrames_)
                {
                    return frames_[pos];
                }
                throw std::out_of_range("StackTrace::at(pos) out of range");
            }

        private:
            //all dbhelp library functions are not thread safe
            //thats why we need a mutext to protect calls to
            //them
            static std::mutex mutex_;
            enum
            {
                cFramesToSkip = 2
            };

            enum
            {
                cFramesSize = 1024
            };
            BacktraceSymbol frames_[cFramesSize];
            std::uint32_t capturedFrames_ = 0;
        };

        std::mutex StackTraceImpl::mutex_;


    } //internal
} //asrt
