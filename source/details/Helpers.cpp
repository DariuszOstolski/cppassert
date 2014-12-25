#include <cppassert/CppAssert.hpp>
#include <cppassert/details/Helpers.hpp>

namespace cppassert
{
namespace internal
{

std::string getBoolAssertionFailureMessage(
    const char* expressionText,
    const char* actualPredicateValue,
    const char* expectedPredicateValue)
{
    return CppAssert::getInstance()->formatBoolFailureMessage(expressionText
                                        , actualPredicateValue
                                        , expectedPredicateValue);
}

std::string getPredicateAssertionFailureMessage(
    const char* predicate,
    const char* value1Text,
    const char* value2Text,
    AssertionMessage &&value1,
    AssertionMessage &&value2)
{
    return CppAssert::getInstance()->formatPredicateFailureMessage(predicate
                                        , value1Text
                                        , value2Text
                                        , value1.str()
                                        , value2.str());
}

std::string getAssertionFailureMessage(const char *statement)
{
    return CppAssert::getInstance()->formatStatementFailureMessage(statement);
}

} //internal
} //asrt



