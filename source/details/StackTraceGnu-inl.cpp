#include <cppassert/details/StackTrace.hpp>
#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <cxxabi.h>
#include <sstream>

namespace cppassert
{
namespace internal
{

/**
 * Provides C++ symbol demangling
 */
class CppDemangler
{
    CppDemangler(const CppDemangler &) = delete;
    CppDemangler &operator=(const CppDemangler &) = delete;
public:
    CppDemangler()
    {
        funcName_ = static_cast<char *>(std::malloc(funcNameSize_));
    }

    ~CppDemangler()
    {
        if(funcName_)
        {
            std::free(funcName_);
        }
    }
    /**
     * Demangle C++ symbol name
     * @param   funcName    C++ mangled symbol name
     * @return  Demangled name for immediate use only
     */
    const char *demangle(const char *funcName)
    {
        std::int32_t status = 0;
        //malloc failed to allocate
        if(funcName_==nullptr)
        {
            return nullptr;
        }

        char* result = abi::__cxa_demangle(funcName,
                                        funcName_, &funcNameSize_, &status);
        if (status == 0)
        {
            //use value that could have been reallocated
            funcName_ = result;
        }
        else
        {
            //demangling failed, just return null
            result = nullptr;
        }
        return result;
    }
private:
    std::size_t funcNameSize_ = 256;
    char* funcName_ = nullptr;
};

/**
 * An intelligent wrapper around symbols that may need or may not need
 * to be deallocated. If symbol was not demangled we shouldn't allocate/deallocate
 * any space just copy a pointer to it. If demangling was successfull it allocates
 * required space and manages lifetime for that space.
 *
 */
class BackTraceSymbol
{
    BackTraceSymbol(const BackTraceSymbol &) = delete;
    BackTraceSymbol &operator=(const BackTraceSymbol &) = delete;
    static constexpr const char *UnkownSymbol = "<unkown>";
public:
    BackTraceSymbol()
    {

    }

    ~BackTraceSymbol()
    {
        deallocate();
    }

    /**
     * Set values for demangled symbols. It allocates required space
     * and makes a deep copy
     *
     * @param backtrace     String returned from backtrace_symbol
     * @param demangledName Demangled symbol name as string
     * @param offset        Symbol offset
     */
    void setSymbol(const char *backtrace
                    , const char *demangledName
                    , const char *offset)
    {
        deallocate();
        std::size_t backTraceLength = std::strlen(backtrace);
        std::size_t nameLength = std::strlen(demangledName);
        std::size_t offsetLength = std::strlen(offset)+1;
        allocated_ = static_cast<char *>(
                        std::malloc(backTraceLength+nameLength+offsetLength+2));
        if(!allocated_)
        {
            return;
        }
        char *begin = allocated_;
        std::strcpy(allocated_, backtrace);
        begin += backTraceLength;
        std::strcpy(begin, "(");
        begin += 1;
        std::strcpy(begin, demangledName);
        begin += nameLength;
        std::strcpy(begin, "+");
        begin += 1;
        std::strcpy(begin, offset);
        symbol_ = allocated_;
    }

    /**
     * Set value to symbol. It's a pointer returned by `backtrace_symbols`
     * function. Only pointer value is copied, life time of this pointer
     * is managed by class that will manage life time of this class
     *
     * @param   symbol  String returned from `backtrace_symbols` function
     *
     */
    void setSymbol(const char *symbol)
    {
        deallocate();
        symbol_ = symbol;
    }

    /**
     * Searches for a symbol in a string returned by backrace function
     * @param[in]   backTraceSymbol String returned from backtrace function
     * @param[out]  beginNamePtr    Position where symbol name begins
     * @param[out]  beginOffsetPtr  Position where offset begins
     * @param[out]  endOffsetPtr    Position where offset ends
     */
    static void findSymbolInBacktraceStr(
                char *backTraceSymbol
                , char **beginNamePtr
                , char **beginOffsetPtr
                , char **endOffsetPtr)
    {
        char *beginName = 0, *beginOffset = 0, *endOffset = 0;

        for (char *position = backTraceSymbol; *position; ++position)
        {
            if (*position == '(')
            {
                beginName = position;
            }
            else if (*position == '+')
            {
                beginOffset = position;
            }
            else if (*position == ')' && beginOffset)
            {
                endOffset = position;
                break;
            }
        }
        *beginNamePtr = beginName;
        *beginOffsetPtr = beginOffset;
        *endOffsetPtr = endOffset;
    }

    /**
     * Parses \p backTraceSymbol string and returns corresponding C++ object
     * @param   backTraceSymbol String returned by `backtrace_symbol` function
     * @param   demangler       Object that will be used to demangle names
     * @return  BackTraceSymbol object
     */
    static BackTraceSymbol createFromBacktraceStr(char *backTraceSymbol
                                        , CppDemangler *demangler)
    {
        BackTraceSymbol result;
        char *beginName = 0, *beginOffset = 0, *endOffset = 0;
        findSymbolInBacktraceStr(backTraceSymbol
                                , &beginName
                                , &beginOffset
                                , &endOffset);

        if (beginName
            && beginOffset
            && endOffset
	    && beginName < beginOffset)
	{
	    *beginName++ = '\0';
            *beginOffset++ = '\0';
            const char *demangledSymbol = demangler->demangle(beginName);
            if(demangledSymbol)
            {
                result.setSymbol(backTraceSymbol, demangledSymbol, beginOffset);
            }
            else
            {
                //restore previous state
                --beginName;
                --beginOffset;
                *beginName = '(';
                *beginOffset = '+';
                result.setSymbol(backTraceSymbol);
            }
        }
        else
        {
            result.setSymbol(backTraceSymbol);
        }

        return result;
    }

    /**
     * Returns value of symbol
     * @return Symbol as string
     */
    const char *symbol() const
    {
        return symbol_;
    }

    /**
     * Move assignment constructor
     * @param   other   Object to be moved
     */
    BackTraceSymbol(BackTraceSymbol && other)
    {
        allocated_ = other.allocated_;
        symbol_ = other.symbol_;
        other.allocated_ = nullptr;
        other.symbol_ = nullptr;
    }

    /**
     * Move assignment operator
     * @param   other   Object to be moved
     */
    BackTraceSymbol &operator=(BackTraceSymbol &&other)
    {
        deallocate();
        allocated_ = other.allocated_;
        symbol_ = other.symbol_;
        other.allocated_ = nullptr;
        other.symbol_ = nullptr;
        return (*this);
    }

private:
    void deallocate()
    {
        if(allocated_)
        {
            std::free(allocated_);
            allocated_ = nullptr;
        }
    }
    const char *symbol_ = UnkownSymbol;
    char *allocated_ = nullptr;
};

/**
 * StackTrace implementation for platforms where backtrace/backtrace_symbols
 * functions are available
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
        freeSymbols();
    }

    /**
     * Collects current stack trace
     */
    void collect()
    {
        freeSymbols();
        CppDemangler demangler;
        backtraceSize_ = ::backtrace(backtrace_, BufferSize);
        symbols_ = ::backtrace_symbols(backtrace_, backtraceSize_);

        for(std::size_t i=cFramesToSkip; i<backtraceSize_; ++i)
        {
            if(symbols_)
            {
                demangledSymbols_[i]
                    = BackTraceSymbol::createFromBacktraceStr(symbols_[i]
                                                            , &demangler);
            }
            else
            {
                demangledSymbols_[i] = BackTraceSymbol();
            }

            frames_[i-cFramesToSkip] = StackTrace::StackFrame(backtrace_[i]
                                            , demangledSymbols_[i].symbol());
        }
    }

    /**
     * Returns number of frames
     * @return  Number of frames
     */
    std::size_t size() const
    {
        return (backtraceSize_-cFramesToSkip);
    }

     /**
     * Returns stack frame at \p position, if position is greater
     * that size `std::out_of_range` exception is thrown
     * @param   position    Number of frame to be returned
     * @return StackFrame at \p position
     */
    const StackTrace::StackFrame &at(std::size_t position) const
    {
        if(position<(backtraceSize_-cFramesToSkip))
        {
            return frames_[position];
        }
        std::stringstream msg;
        msg << "StackFrame::at: Position "<<position
            << " is out of range: 0 and "<<size();
        throw std::out_of_range(msg.str());
        //make compiler happy
        return frames_[0];
    }

private:
    enum
    {
        BufferSize = 256
    };

    enum
    {
        cFramesToSkip = 2
    };

    void freeSymbols()
    {
        if(symbols_)
        {
            std::free(symbols_);
        }
    }

    char **symbols_ = nullptr;
    void *backtrace_[BufferSize];
    StackTrace::StackFrame frames_[BufferSize];
    BackTraceSymbol demangledSymbols_[BufferSize];
    std::size_t backtraceSize_ = 0;
};



} //internal
} //asrt


