#pragma once

#include <sne/srpc/RpcInterface.h>
#include <sne/srpc/RpcTypes.h>

/**
 * @class Echo
 */
class Echo
{
public:
    virtual ~Echo() {}

    DECLARE_SRPC_METHOD_1(Echo, echo, std::string, data);

    DECLARE_SRPC_METHOD_1(Echo, onEcho, std::string, data);
};
