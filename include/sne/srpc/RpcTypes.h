#pragma once

#include <sne/Common.h>
#include <sne/core/stream/IStream.h>
#include <sne/core/stream/OStream.h>
#include <type_traits>

namespace sne { namespace srpc {

/** @addtogroup Stream
* @{
*/

/**
 * @class RpcIngerType
 * Rpc 숫자 타입 base template class
 * - StreamingType은 실제로 streaming할 때 쓰이는 타입으로 enum을 부호없는
 *   정수로 취급할 때 쓰인다.
 */
template <typename NativeType, int bits = core::Bits<NativeType>::size,
    typename StreamingType = NativeType>
class RpcNumericType
{
public:
    static int getBits() { return bits; }

public:
    RpcNumericType() {}
    RpcNumericType(const RpcNumericType& t) :
        value_(t.value_) {}
    RpcNumericType(NativeType v) :
        value_(v) {}

    void operator=(const RpcNumericType& t) {
        value_ = t.value_;
    }

    operator NativeType() const {
        return value_;
    }

    NativeType get() const {
        return value_;
    }

    void set(NativeType v) {
        value_ = v;
    }

    const NativeType& ref() const {
        return value_;
    }

    NativeType& ref() {
        return value_;
    }

    void serialize(core::OStream& ostream) {
        ostream.write(static_cast<StreamingType>(value_), bits);
    }

    void serialize(core::IStream& istream) {
        typedef std::integral_constant<bool,
            std::is_enum<StreamingType>::value> truth_type;
        StreamingType v;
        istream.do_read(v, bits, truth_type());
        value_ = static_cast<NativeType>(v);
    }

private:
    NativeType value_;
};


/// @deprecated Rpc 부호 없는 숫자 타입 base template class
template <typename NativeType, size_t bits = core::Bits<NativeType>::size,
    typename StreamingType = NativeType>
class RpcUIntType : public RpcNumericType<NativeType, bits, StreamingType>
{
public:
    RpcUIntType() {}
    RpcUIntType(const RpcUIntType& t) :
        RpcNumericType<NativeType, bits, StreamingType>(t.get()) {}
    RpcUIntType(NativeType v) :
        RpcNumericType<NativeType, bits, StreamingType>(v) {}
};


/// @deprecated Rpc 부호 있는 숫자 타입 base template class
template <typename NativeType, size_t bits = core::Bits<NativeType>::size,
    typename StreamingType = NativeType>
class RpcIntType : public RpcNumericType<NativeType, bits, StreamingType>
{
public:
    RpcIntType() {}
    RpcIntType(const RpcIntType& t) :
        RpcNumericType<NativeType, bits, StreamingType>(t.get()) {}
    RpcIntType(NativeType v) :
        RpcNumericType<NativeType, bits, StreamingType>(v) {}
};


/**
 * Rpc String Type
 * - (maxLength >= 0) && (maxLength <= 65535)
 * - (sizeBits == 8) || (sizeBits == 16)
 * - maxLength가 256 보다 작은 경우 1바이트를 문자열 길이 필드로 사용한다.
 * - maxLength가 65536 보다 작은 경우 2바이트를 문자열 길이 필드로 사용한다.
 * - 유니코드인 경우 maxLength에 따라 sizeBits를 좀 더 정확하게 설정해야 한다.
 */
template <typename StringType, size_t maxLength,
    int sizeBits = (maxLength > UCHAR_MAX) ?
        core::Bits<uint16_t>::size : SNE_DEFAULT_STRING_SIZE_BITS>
class RpcStringType : public StringType
{
public:
    static size_t getMaxLength() { return maxLength; }
    static size_t getSizeBits() { return sizeBits; }

public:
    typedef typename StringType::value_type value_type;
    typedef StringType NativeType;

public:
    RpcStringType() {}
    RpcStringType(const StringType& s) :
        StringType(s) {}
    RpcStringType(const value_type* s) :
        StringType(s) {}

    void operator=(const StringType& s) {
        StringType::operator=(s);
    }

    void operator=(const value_type* s) {
        StringType::operator=(s);
    }

    const StringType& ref() const {
        return *this;
    }

    StringType& ref() {
        return *this;
    }

    void serialize(core::OStream& ostream) {
        static_assert((maxLength >= 0) && (maxLength <= USHRT_MAX), "");
        static_assert((sizeBits == 8) || (sizeBits == 16), "");
        ostream.write(*this, maxLength, sizeBits);
    }

    void serialize(core::IStream& istream) {
        static_assert((maxLength >= 0) && (maxLength <= USHRT_MAX), "");
        static_assert((sizeBits == 8) || (sizeBits == 16), "");
        istream.read(*this, maxLength, sizeBits);
    }
};


// = basic Rpc Primitive Types

typedef RpcNumericType<bool, core::Bits<bool>::size> RBool;

typedef RpcNumericType<int8_t> RInt8;
typedef RpcNumericType<uint8_t> RUInt8;

typedef RpcNumericType<int16_t> RInt16;
typedef RpcNumericType<uint16_t> RUInt16;

typedef RpcNumericType<int32_t> RInt32;
typedef RpcNumericType<uint32_t> RUInt32;

typedef RpcNumericType<int64_t> RInt64;
typedef RpcNumericType<uint64_t> RUInt64;

typedef RpcNumericType<float32_t> RFloat32;
typedef RpcNumericType<float64_t> RFloat64;

typedef RpcStringType<std::string, USHRT_MAX> RString;
typedef RpcStringType<std::string, UCHAR_MAX> RShortString;

typedef RpcStringType<std::wstring, USHRT_MAX> RWString;
typedef RpcStringType<std::wstring, UCHAR_MAX> RWShortString;

/** @} */ // addtogroup Stream

}} // namespace sne { namespace srpc

// 참고: http://stackmobile.com/stackoverflow/questions/32685540
namespace std {

    template <>
    struct hash<sne::srpc::RString>
    {
        size_t operator()(sne::srpc::RString const& v) const {
            std::hash<const char*> h;
            return h(v.c_str());
        }
    };

    template <>
    struct hash<sne::srpc::RWString>
    {
        size_t operator()(sne::srpc::RWString const& v) const {
            std::hash<const wchar_t*> h;
            return h(v.c_str());
        }
    };

    template <size_t maxLength, int sizeBits>
    struct hash<sne::srpc::RpcStringType<std::string, maxLength, sizeBits>>
    {
        size_t operator()(sne::srpc::RpcStringType<std::string, maxLength> const& v) const {
            std::hash<const char*> h;
            return h(v.c_str());
        }
    };

    template <size_t maxLength, int sizeBits>
    struct hash<sne::srpc::RpcStringType<std::wstring, maxLength, sizeBits>>
    {
        size_t operator()(sne::srpc::RpcStringType<std::wstring, maxLength> const& v) const {
            std::hash<const wchar_t*> h;
            return h(v.c_str());
        }
    };

} // namespace std
