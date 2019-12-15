#pragma once

#include <sne/Common.h>
#include "Stream.h"
#include <sne/core/utility/Bits.h>
#include <type_traits>

namespace sne { namespace core {

class OStream;

/** @addtogroup Stream
* @{
*/

namespace detail
{

/// 기본 데이터형이 아닌 것은 무조건 RpcType으로 가정
template <typename T, bool isFundamental, bool isEnum>
struct StreamWriterImpl;

} // namespace detail

/**
 * @class OStream
 *
 * Output stream.
 * - CAUTION: 데이터 타입을 기준으로 스트림에 출력한다
 */
class OStream : public Stream
{
public:
    virtual bool isInput() const override {
        return false;
    }

    virtual bool isOutput() const override {
        return true;
    }

public:
    /// for RPC enum data type
    //void write(int value, int bitCount) {
    //    write(static_cast<int32_t>(value), bitCount);
    //}

    /// 64비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(uint64_t value, int bitCount = Bits<uint64_t>::size) = 0;

    /// 32비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(uint32_t value, int bitCount = Bits<uint32_t>::size) = 0;

    /// 16비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(uint16_t value, int bitCount = Bits<uint16_t>::size) = 0;

    /// 8비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(uint8_t value, int bitCount = Bits<uint8_t>::size) = 0;

    /// 부울린 값을 스트림버퍼에 기록한다
    virtual void write(bool value, int bitCount = Bits<bool>::size) = 0;

    /**
     * 64비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(int64_t value, int bitCount = Bits<int64_t>::size) = 0;

    /**
    * 32비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
    * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
    */
    virtual void write(int32_t value, int bitCount = Bits<int32_t>::size) = 0;

    /**
     * 16비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(int16_t value, int bitCount = Bits<int16_t>::size) = 0;

    /**
     * 8비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(int8_t value, int bitCount = Bits<int8_t>::size) = 0;

    /**
     * 32비트 부호 있는 부동 소수점 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(float32_t value, int bitCount = Bits<int32_t>::size) = 0;

    virtual void writeEnum(uint64_t value, int bitCount) = 0;

    /// 최대 maxLength 길이의 문자열을 스트림버퍼에 기록한다
    virtual void write(const std::string& value, size_t maxLength,
        int sizeBitCount) = 0;

    /// 최대 maxLength 길이의 문자열을 스트림버퍼에 기록한다
    virtual void write(const std::wstring& value, size_t maxLength,
        int sizeBitCount) = 0;

    /**
     * raw buffer를 스트림버퍼에 기록한다
     * @param buffer raw buffer
     * @param length 버퍼의 바이트수(USHRT_MAX 보다 작거나 같아야 한다)
     */
    virtual void write(const void* buffer, uint16_t length) = 0;

    /// 패딩을 추가하여 바이트 단위로 정렬한다
    virtual void align() = 0;

    /**
     * 비트 스트림을 초기화한다
     * @param resetBuffer 버퍼를 초기화할 것인가?
     */
    virtual void reset(bool resetBuffer) = 0;

    /// 저장된 데이터의 크기(비트 단위)를 얻는다
    virtual size_t getTotalBitCount() const = 0;

    /// 저장된 데이터의 크기(바이트 단위)를 얻는다
    virtual size_t getTotalSize() const = 0;
};


/// TODO: value타입에 레퍼런스를 사용하지 않을 경우 RpcType이 값으로 넘어오는
///       문제가 있다. 이 문제를 해결하기 위해 레퍼런스를 쓰긴 했지만,
///       그로인해 Primitive-type이 레퍼런스로 넘어오는 다른 문제가 생김.
///       최적화 방법 없나?
template <typename T>
inline OStream& operator<<(OStream& lhs, const T& value) {
    typedef typename std::remove_reference<T>::type RawType;

    detail::StreamWriterImpl<T, std::is_fundamental<RawType>::value,
        std::is_enum<RawType>::value>::write(lhs, value);
    return lhs;
}

namespace detail {

// fundamental type
template <typename T>
struct StreamWriterImpl<T, true, false>
{
    static void write(OStream& ostream, T value) {
        ostream.write(value);
    }
};

// enum type
template <typename T>
struct StreamWriterImpl<T, false, true>
{
    static void write(OStream& ostream, T value) {
        ostream.writeEnum(static_cast<uint64_t>(value), Bits<T>::size);
    }
};


// String type
template <>
struct StreamWriterImpl<std::string, false, false>
{
    static void write(OStream& ostream, const std::string& value) {
        ostream.write(value, USHRT_MAX, Bits<uint16_t>::size);
    }
};


// std::wstring type
template <>
struct StreamWriterImpl<std::wstring, false, false>
{
    static void write(OStream& ostream, const std::wstring& value) {
        ostream.write(value, USHRT_MAX, Bits<uint16_t>::size);
    }
};


// RPC type
template <typename T>
struct StreamWriterImpl<T, false, false>
{
    static void write(OStream& ostream, const T& value) {
        const_cast<typename std::remove_const<T>::type&>(value).serialize(ostream);
    }
};

} // namespace detail

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core
