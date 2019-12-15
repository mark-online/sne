#pragma once

#include "../detail/RpcId.h"
#include <sne/core/stream/Streamable.h>
#include <sne/Common.h>
#include <boost/utility.hpp>

namespace sne { namespace core {
class OStream;
}} // namespace sne { namespace core {

namespace sne { namespace srpc {

class ForwardingFunctor;

/** @addtogroup RpcForwarding
* @{
*/

/**
 * @class RpcCommand
 *
 * rpc command(request)
 * - 요청하는 쪽의 부하를 줄이기 위해 필요한 정보를 wrapping
 */
class SNE_API RpcCommand : public core::Streamable
{
public:
    RpcCommand(const RRpcId& rpcId, ForwardingFunctor& marshaler)  :
        rpcId_(rpcId),
        marshaler_(marshaler) {}

    virtual ~RpcCommand() {}

    /**
     * RPC command를 ostream으로 마샬링한다
     * @exception StreamingException 스트리밍에 실패할 경우
     */
    void marshal(core::OStream& ostream);

    const RRpcId& getRpcId() const {
        return rpcId_;
    }

private:
    // = core::Streamable overriding

    /**
     * RPC command를 ostream으로 마샬링한다
     * @exception StreamingException 스트리밍에 실패할 경우
     */
    virtual void serialize(core::Stream& stream) override;

private:
    const RRpcId& rpcId_;
    ForwardingFunctor& marshaler_;
};

/** @} */ // addtogroup RpcForwarding

}} // namespace sne { namespace srpc
