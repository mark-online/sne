#pragma once

#include "ReceivingFunctors.h"
#include "RpcId.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace sne { namespace core {
class IStream;
}} // namespace sne { namespace core {

namespace sne { namespace srpc {

/** @addtogroup RpcReceiving
* @{
*/

/**
 * @class RpcEvent
 *
 * rpc event(response)
 */
class RpcEvent : public boost::noncopyable
{
public:
    RpcEvent(const RRpcId& rpcId) :
        rpcId_(rpcId) {}

    virtual ~RpcEvent() {}

    void dispatch(void* rpcClassThisPtr, core::IStream& istream,
        const void* rpcHint = nullptr) {
        ReceivingFunctor& dispatcher = getDispatcher();
        dispatcher.unmarshal(istream);
        dispatcher.call(rpcClassThisPtr, rpcHint);
    }

    const RRpcId& getRpcId() const {
        return rpcId_;
    }
public:
    virtual std::unique_ptr<RpcEvent> clone() const = 0;

    virtual ReceivingFunctor& getDispatcher() = 0;
private:
    const RRpcId& rpcId_;
};

/** @} */ // addtogroup RpcReceiving

}} // namespace sne { namespace srpc
