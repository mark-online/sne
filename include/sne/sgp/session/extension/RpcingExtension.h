#pragma once

#include <sne/base/session/extension/SessionExtension.h>
#include <sne/Common.h>

namespace sne { namespace srpc {
class RpcForwarder;
class RpcReceiver;
}} // namespace sne { namespace srpc {

namespace sne { namespace sgp {

namespace detail {
class RpcingExtensionImpl;
} // namespace detail

/** @addtogroup session
* @{
*/

/**
 * @class RpcingExtension
 */
class SNE_API RpcingExtension : public base::AbstractSessionExtension<RpcingExtension>
{
    SNE_GENERATE_SESSION_EXTENSION_ID(RpcingExtension);

public:
    RpcingExtension();
    virtual ~RpcingExtension();

    /// RpcForwarder를 등록한다.
    void registerRpcForwarder(srpc::RpcForwarder& forwarder);

    /// RpcForwarder의 등록을 해제한다
    void unregisterRpcForwarder(srpc::RpcForwarder& forwarder);

    /// RpcReceiver를 등록한다.
    void registerRpcReceiver(srpc::RpcReceiver& receiver);

    /// RpcReceiver의 등록을 해제한다
    void unregisterRpcReceiver(srpc::RpcReceiver& receiver);

public:
    virtual void attachedTo(base::SessionImpl& sessionImpl) override;

private:
    virtual bool handleMessage(base::MessageType messageType,
        base::MemoryBlock& mblock) override;

private:
    std::unique_ptr<detail::RpcingExtensionImpl> impl_;
};

/** @} */ // addtogroup session

}} // namespace sne { namespace sgp {
