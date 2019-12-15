#pragma once

#include <sne/Common.h>
#include <boost/predef/other/endian.h>
#include <cassert>

// TODO: big-endian system에서 테스트

namespace sne { namespace core {

namespace detail {

// primary template
template <typename T, std::size_t s = sizeof(T)>
struct RpcByteOrderImpl;

// 1 byte
template <typename T>
struct RpcByteOrderImpl<T, 1>
{
    static T convert(T value) {
        return value;
    }
};

// 2 bytes
template <typename T>
struct RpcByteOrderImpl<T, 2>
{
    static T convert(T value) {
        uint8_t* src = static_cast<uint8_t*>(static_cast<void*>(&value));
        uint8_t* dest = src + sizeof(T) - 1;
        *dest-- = *src++;
        *dest = *src;
        return value;
    }
};

// 4 bytes
template <typename T>
struct RpcByteOrderImpl<T, 4>
{
    static T convert(T value) {
        uint8_t* src = static_cast<uint8_t*>(static_cast<void*>(&value));
        uint8_t* dest = src + sizeof(T) - 1;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest = *src;
        return value;
    }
};

// 8 bytes
template <typename T>
struct RpcByteOrderImpl<T, 8>
{
    static T convert(T value) {
        uint8_t* src = static_cast<uint8_t*>(static_cast<void*>(&value));
        uint8_t* dest = src + sizeof(T) - 1;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest = *src;
        return value;
    }
};

} // namespace detail

/**
 * convert to RPC byte-order(little-endian)
 */
template <typename T>
inline T toRpcByteOrder(T value)
{
#if BOOST_ENDIAN_BIG_BYTE
    return detail::RpcByteOrderImpl<T>::convert(value);
#elif !BOOST_ENDIAN_LITTLE_BYTE
    assert(false && "unknown byte-order");
#endif
    return value;
}

}} // namespace sne { namespace core
