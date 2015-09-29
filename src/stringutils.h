#include <string>
#include <type_traits>

namespace StringUtils
{
std::string repeated(const std::string& str, unsigned int times);

std::string trimmed(const std::string& str);

std::string simplified(const std::string& str);

std::string fromBase64(const std::string& str);

std::string toBase64(const std::string& str);

std::string fromHex(const std::string& str);

std::string toHex(const std::string& str);

template<class T, class D, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0, typename std::enable_if<std::is_floating_point<D>::value, int>::type = 0>
bool approximatelyEqual(T lhs, D rhs, typename std::common_type<T, D>::type precision)
{
    typename std::common_type<T, D>::type diff = lhs - rhs;

    if(diff < 0)
    {
        diff = -diff;
    }

    return diff < precision;
}
};

