// C++ related utilities

#pragma once

#include <sne/Common.h>
#include <sne/core/utility/CUtils.h>
#include <boost/functional/hash/hash.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <functional>
#include <algorithm>

namespace sne { namespace core {

inline std::string formatString(const char *format, ...)
{
    const size_t maxlen = 1024;
    char buffer[maxlen + 1];

    va_list ap;
    va_start(ap, format);
    const int result = vsnprintf(buffer, maxlen, format, ap);
    va_end(ap);

    if (result > 0) {
        return std::string(buffer);
    }
    return std::string();
}


inline std::wstring formatString(const wchar_t *format, ...)
{
    const size_t maxlen = 1024;
    wchar_t buffer[maxlen + 1];

    va_list ap;
    va_start(ap, format);
    const int result = vsnprintf(buffer, maxlen, format, ap);
    va_end(ap);

    if (result > 0) {
        return std::wstring(buffer);
    }
    return std::wstring();
}


/// 연관 컨테이너(map, multimap, hash_map)에서 항목을 검사한 후 값을 리턴한다
template <class Map>
inline typename Map::value_type::second_type search_map(const Map& the_map,
    const typename Map::value_type::first_type& key,
    const typename Map::value_type::second_type& null_result =
        Map::value_type::second_type())
{
    typename Map::const_iterator pos = the_map.find(key);
    if (pos != the_map.end()) {
        return (*pos).second;
    }
    return null_result;
}


/// 대소문자 구분하지 않는 equal_to (문자열 전용)
template <typename T>
struct iequal_to : public std::binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        if (x.size() != y.size()) {
            return false;
        }
        return boost::algorithm::iequals(x, y, std::locale());
    }
};


/// 대소문자 구분하지 않는 hash (문자열 전용)
template <typename T>
struct ihash : public std::unary_function<T, std::size_t>
{
    std::size_t operator()(const T& x) const {
        std::size_t seed = 0;
        std::locale locale;

        typename T::const_iterator pos = x.begin();
        const typename T::const_iterator end = x.end();
        for(; pos != end; ++pos) {
            boost::hash_combine(seed, std::toupper(*pos, locale));
        }

        return seed;
    }
};

}} // namespace sne { namespace core {
