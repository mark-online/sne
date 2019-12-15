#pragma once

#include <sne/core/Exception.h>

namespace sne { namespace srpc {

/** @addtogroup Exception
* @{
*/

/**
 * @class UnknownRpcMethodException
 *
 * 알 수 없는 RPC 요청을 수신한 경우
 */
class UnknownRpcMethodException : public core::Exception
{
public:
    UnknownRpcMethodException(const char* file, int fileno, const char* what) :
        core::Exception(file, fileno, what) {}
};


/**
* @class RpcFailedException
*
* RPC 요청을 처리하는 도중 에러가 발생할 경우
* - 접속을 해제해야 할 경우 throw.
*/
class RpcFailedException : public core::Exception
{
public:
    RpcFailedException(const char* file, int fileno, const char* what) :
        core::Exception(file, fileno, what) {}
};

/** @} */ // addtogroup Exception

}} // namespace sne { namespace srpc
