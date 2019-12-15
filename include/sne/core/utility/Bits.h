#pragma once

#include <climits>
#include <cassert>

namespace sne { namespace core {

/// get bit-count of type T
template <typename T>
struct Bits
{
    static const int size = (sizeof(T) * CHAR_BIT);
};


/// get bit-count of type bool
template <>
struct Bits<bool>
{
    static const int size = 1;
};


/// 비트를 채우기 위한 바이트수를 구한다
template <typename T>
inline T toBytes(T bitCount)
{
    if (bitCount < 0) {
        return 0;
    }
    return (bitCount + (CHAR_BIT - 1)) / CHAR_BIT;
}


/// 스트리밍할 수 있는 비트수인가?
template <typename T>
inline bool isEnoughBits(int bitCount)
{
    return (0 < bitCount) && (bitCount <= Bits<T>::size);
}


/// bitCount비트 만큼의 부호없는 정수가 표현할 수 있는 최대값을 얻는다.
template <size_t bitCount>
struct MaxNumber {
    enum {
        power = 2 * MaxNumber<bitCount - 1>::power,
        value = (2 * power) - 1
    };
};


template <>
struct MaxNumber<1> {
    enum {
        power = 1,
        value = 1
    };
};


template <>
struct MaxNumber<0> {
    enum {
        power = 0,
        value = 0
    };
};


/// bitCount비트 만큼의 부호없는 정수가 표현할 수 있는 최대값을 얻는다.
inline size_t getMaxNumberOf(size_t bitCount)
{
    size_t maxNumber = 1;
    for (size_t b = 0; b < bitCount; ++b) {
        maxNumber *= 2;
    }
    return maxNumber - 1;
}

}} // namespace sne { namespace core
