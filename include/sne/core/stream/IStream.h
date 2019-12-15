#pragma once

#include <sne/Common.h>
#include "Stream.h"
#include <sne/core/utility/Bits.h>
#include <type_traits>

namespace sne { namespace core {

class IStream;

/** @addtogroup Stream
* @{
*/

namespace detail
{

/// 기본 데이터형이 아닌 것은 무조건 RpcType으로 가정
template <typename T, bool isFundamental, bool isEnum>
struct StreamReaderImpl;

} // namespace detail

/**
 * @class IStream
 *
 * Input stream.
 * - CAUTION: 데이터 타입을 기준으로 스트림에서 읽어온다
 */
class IStream : public Stream
{
public:
    virtual bool isInput() const override {
        return true;
    }

    virtual bool isOutput() const override {
        return false;
    }

public:
    /// 스트림버퍼로 부터 64비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(uint64_t& value, int bitCount = Bits<uint64_t>::size) = 0;

    /// 스트림버퍼로 부터 32비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(uint32_t& value, int bitCount = Bits<uint32_t>::size) = 0;

    /// 스트림버퍼로 부터 16비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(uint16_t& value, int bitCount = Bits<uint16_t>::size) = 0;

    /// 스트림버퍼로 부터 8비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(uint8_t& value, int bitCount = Bits<uint8_t>::size) = 0;

    /// 스트림버퍼로 부터 64비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(int64_t& value, int bitCount = Bits<int64_t>::size) = 0;

    /// 스트림버퍼로 부터 32비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(int32_t& value, int bitCount = Bits<int32_t>::size) = 0;

    /// 스트림버퍼로 부터 16비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(int16_t& value, int bitCount = Bits<int16_t>::size) = 0;

    /// 스트림버퍼로 부터 8비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(int8_t& value, int bitCount = Bits<int8_t>::size) = 0;

    /// 스트림버퍼로 부터 부울린값을 읽어 온다
    virtual void read(bool& value, int bitCount = Bits<bool>::size) = 0;

    /// 스트림버퍼로 부터 32비트 부동 소숫점 타입을 읽어 온다
    virtual void read(float32_t& value, int bitCount = Bits<int32_t>::size) = 0;

    virtual void readEnum(uint64_t& value, int bitCount) = 0;

    /// 최대 maxLength 길이의 문자열을 읽어온다
    virtual void read(std::string& value, size_t maxLength,
        int sizeBitCount) = 0;

    /// 최대 maxLength 길이의 문자열을 읽어온다
    virtual void read(std::wstring& value, size_t maxLength,
        int sizeBitCount) = 0;

    /**
     * 스트림버퍼로 부터 raw buffer를 읽어 온다
     * @param buffer 입력 버퍼. 반드 length 이상의 메모리를 확보해야 한다.
     * @param length 읽어올 바이트 수
     */
    virtual void read(void* buffer, uint16_t length) = 0;

    /// 스트림을 바이트 단위로 정렬한다(버퍼에 남아있는 비트를 버린다)
    virtual void align() = 0;

    /**
     * 스트림을 초기화한다.
     * @param resetBuffer 버퍼를 초기화할 것인가?
     */
    virtual void reset(bool resetBuffer) = 0;

    /// 버퍼에 남아 있는 데이터의 총 크기(비트 단위)를 얻는다
    virtual size_t getTotalBitCount() const = 0;

    /// 버퍼에 남아 있는 데이터의 총 크기(바이트 단위)를 얻는다
    virtual size_t getTotalSize() const = 0;

    template <typename T>
    void do_read(T& value, int sizeBitCount, const std::false_type&) {
        read(value, sizeBitCount);
    }

    template <typename T>
    void do_read(T& value, int sizeBitCount, const std::true_type&) {
        int32_t temp;
        read(temp, sizeBitCount);
        value = static_cast<T>(temp);
    }
};


template <typename T>
inline IStream& operator>>(IStream& lhs, T& value) {
    detail::StreamReaderImpl<T, std::is_fundamental<T>::value,
        std::is_enum<T>::value>::read(lhs, value);
    return lhs;
}


namespace detail {

// fundamental type
template <typename T>
struct StreamReaderImpl<T, true, false>
{
    static void read(IStream& istream, T& value) {
        istream.read(value);
    }
};

// enum type
template <typename T>
struct StreamReaderImpl<T, false, true>
{
    static void read(IStream& istream, T& value) {
        uint64_t aValue;
        istream.readEnum(aValue, Bits<T>::size);
        value = static_cast<T>(aValue);
    }
};


// String type
template <>
struct StreamReaderImpl<std::string, false, false>
{
    static void read(IStream& istream, std::string& value) {
        istream.read(value, USHRT_MAX, Bits<uint16_t>::size);
    }
};


// std::wstring type
template <>
struct StreamReaderImpl<std::wstring, false, false>
{
    static void read(IStream& istream, std::wstring& value) {
        istream.read(value, USHRT_MAX, Bits<uint16_t>::size);
    }
};

// RPC type
template <typename T>
struct StreamReaderImpl<T, false, false>
{
    static void read(IStream& istream, T& value) {
        value.serialize(istream);
    }
};

} // namespace detail

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core
