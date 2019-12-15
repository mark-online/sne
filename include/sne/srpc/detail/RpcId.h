#pragma once

#include "../RpcTypes.h"
#include <sne/core/utility/Hash.h>
#include <cstring>

namespace sne { namespace srpc {

/// RPC ID
typedef uint32_t RpcId;

/**
 * @class RRpcId
 * RPC type for RPC ID
 */
class RRpcId : public RpcNumericType<RpcId>
{
public:
    explicit RRpcId(RpcId rpcId = 0) {
        set(rpcId);
    }

    explicit RRpcId(const std::string& id) {
        set(core::hash(id.c_str(), id.size()));
    }

    RRpcId(const char* id, const char* methodName) :
        methodName_(methodName) {
        set(core::hash(id, strlen(id)));
    }

    const char* getMethodName() const {
        return methodName_;
    }
private:
    const char* methodName_;
};

}} // namespace sne { namespace srpc
