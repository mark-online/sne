#pragma once

#include <sne/base/utility/Assert.h>
#include <sne/srpc/RpcContainers.h>
#include <sne/srpc/RpcTypes.h>
#include <sne/core/container/Containers.h>

namespace sne { namespace database { namespace proxy {

typedef uint32_t RequestId;
const RequestId invalidRequestId = 0;


/**
 * @enum ParamType
 * 파라미터 타입
 */
enum ParamType
{
    ptUnknown = 0,
    ptInt32,
    ptUInt32,
    ptInt64,
    ptUInt64,
    ptString,
    ptWString
};


inline bool isValid(ParamType pt)
{
    return ptUnknown != pt;
}


/**
 * @enum RequestPriority
 * - 잡다한 로그
 */
enum RequestPriority
{
    rpLowest = 0,
    rpLow,
    rpNormal,
    rpHigh,
    rpHighest
};


inline bool isValid(RequestPriority rp)
{
    return (rpLowest <= rp) && (rp <= rpHighest);
}


/**
 * @struct Param
 * type_에 따라 사용되는 value가 달라진다
 */
struct Param
{
    ParamType type_;
    int32_t int32Value_;
    uint32_t uint32Value_;
    int64_t int64Value_;
    uint64_t uint64Value_;
    srpc::RString stringValue_;
    srpc::RWString wstringValue_;

    Param() :
        type_(ptUnknown),
        int32Value_(0),
        int64Value_(0) {}

    explicit Param(int32_t value) :
        type_(ptInt32),
        int32Value_(value),
        uint32Value_(0),
        int64Value_(0),
        uint64Value_(0) {}

    explicit Param(uint32_t value) :
        type_(ptUInt32),
        uint32Value_(value),
        int32Value_(0),
        int64Value_(0),
        uint64Value_(0) {}

    explicit Param(int64_t value) :
        type_(ptInt64),
        int64Value_(value),
        int32Value_(0),
        uint32Value_(0),
        uint64Value_(0) {}

    explicit Param(uint64_t value) :
        type_(ptUInt64),
        uint64Value_(value),
        int32Value_(0),
        uint32Value_(0),
        int64Value_(0) {}

    explicit Param(const std::string & value) :
        type_(ptString),
        stringValue_(value),
        int32Value_(0),
        uint32Value_(0),
        int64Value_(0),
        uint64Value_(0) {}

    explicit Param(const std::wstring & value) :
        type_(ptWString),
        wstringValue_(value),
        int32Value_(0),
        uint32Value_(0),
        int64Value_(0),
        uint64Value_(0) {}

    bool isValid() const {
        return proxy::isValid(type_);
    }

    friend std::wostream& operator<<(std::wostream& lhs, const Param& rhs) {
        switch (rhs.type_) {
        case ptInt32:
            lhs << rhs.int32Value_;
            break;
        case ptUInt32:
            lhs << rhs.uint32Value_;
            break;
        case ptInt64:
            lhs << rhs.int64Value_;
            break;
        case ptUInt64:
            lhs << rhs.uint64Value_;
            break;
        case ptString:
            lhs << rhs.stringValue_.c_str();
            break;
        case ptWString:
            lhs << rhs.wstringValue_.c_str();
            break;
        default:
            SNE_ASSERT(false && "invalid param type");
        }
        return lhs;
    }

    template <typename Stream>
    void serialize(Stream& stream) {
        if (stream.isOutput()) {
            uint8_t paramType = static_cast<uint8_t>(type_);
            stream & paramType;
        }
        else {
            uint8_t paramType;
            stream & paramType;
            type_ = static_cast<ParamType>(paramType);
        }

        switch (type_) {
        case ptInt32:
            stream & int32Value_;
            break;
        case ptUInt32:
            stream & uint32Value_;
            break;
        case ptInt64:
            stream & int64Value_;
            break;
        case ptUInt64:
            stream & uint64Value_;
            break;
        case ptString:
            stream & stringValue_;
            break;
        case ptWString:
            stream & wstringValue_;
            break;
        default:
            SNE_ASSERT(false && "invalid param type");
        }
    }
};


/// array of Param
typedef srpc::RVector<Param> Params;


/**
 * @enum RequestType
 * 요청 타입
 */
enum RequestType
{
    rtUnknown = 0,
    rtStoredProcedure,
    rtSelect /// 현재 지원하지 않음
};


inline bool isValid(RequestType rt)
{
    return rtUnknown != rt;
}


/**
 * @struct ModifyRequest
 */
struct ModifyRequest
{
    RequestPriority priority_;
    RequestType type_;
    srpc::RShortString command_;
    Params params_;

    ModifyRequest() :
        type_(rtUnknown) {}

    ModifyRequest(RequestPriority priority, RequestType type,
        const std::string & command, const Params& params = Params()) :
        priority_(priority),
        type_(type),
        command_(command),
        params_(params) {}

    bool isValid() const {
        return proxy::isValid(priority_) && proxy::isValid(type_) &&
            (! command_.empty()) && (! params_.empty());
    }

    std::wstring  toString() const {
        std::wostringstream oss;
        Params::const_iterator pos = params_.begin();
        const Params::const_iterator end = params_.end();
        for (; pos != end; ++pos) {
            const Param& param = *pos;
            if (pos != params_.begin()) {
                oss << L",";
            }
            oss << L"(" << param.type_ << L"," << param << L")";
        }
        return oss.str();
    }

    template <typename T>
    ModifyRequest& operator<<(T value) {
        params_.push_back(Param(value));
        return *this;
    }

    ModifyRequest& operator<<(const std::string & value) {
        params_.push_back(Param(value));
        return *this;
    }

    ModifyRequest& operator<<(const std::wstring & value) {
        params_.push_back(Param(value));
        return *this;
    }

    template <typename Stream>
    void serialize(Stream& stream) {
        if (stream.isOutput()) {
            stream & static_cast<uint8_t>(priority_) &
                static_cast<uint8_t>(type_);
        }
        else {
            uint8_t priority;
            uint8_t type;
            stream & priority & type;
            priority_ = static_cast<RequestPriority>(priority);
            type_ = static_cast<RequestType>(type);
        }
        stream & command_ & params_;
    }
};

}}} // namespace sne { namespace database { namespace proxy {
