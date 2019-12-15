#include "ServerPCH.h"
#include <sne/server/s2s/ServerSideProxy.h>
#include <sne/server/s2s/ServerSideBridge.h>
#include <sne/server/utility/Profiler.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/Assert.h>

namespace sne { namespace server {

ServerSideProxy::ServerSideProxy(ServerSideBridge* bridge) :
    serverId_(ServerId::invalid),
    bridge_(bridge)
{
    if (bridge_ != nullptr) {
        bridge_->setServerSideBridgeCallback(this);
    }
}


ServerSideProxy::~ServerSideProxy()
{
}


base::Session& ServerSideProxy::getSession()
{
    return *bridge_;
}


// = ServerSideBridgeCallback overriding

void ServerSideProxy::connected()
{
}


void ServerSideProxy::disconnected()
{
    assert(bridge_ != nullptr);

    const base::InetAddress remoteAddress = bridge_->getImpl().getRemoteAddress();
    SNE_LOG_INFO("S2S: ServerSideProxy(S%u,%s:%u) disconnected.",
        serverId_, remoteAddress.address_.c_str(), remoteAddress.port_);

    unbridged();
    serverId_ = ServerId::invalid;
}


void ServerSideProxy::destroyed()
{
    boost::checked_delete(this);
}


void ServerSideProxy::authenticate(const S2sCertificate& certificate)
{
    assert(bridge_ != nullptr);

    Profiler profiler(__FUNCTION__);

    serverId_ = static_cast<ServerId>(bridge_->getClientId());

    const bool isAuthenticated = verifyAuthentication(certificate);
    bridge_->onAuthenticate(isAuthenticated);

    if (! isAuthenticated) {
        serverId_ = ServerId::invalid;
        bridge_->disconnectGracefully();
        return;
    }

    bridged();

    const base::InetAddress remoteAddress = bridge_->getImpl().getRemoteAddress();
    SNE_LOG_INFO("S2S: ServerSideProxy(S%u,%s:%u) connected.",
        serverId_, remoteAddress.address_.c_str(), remoteAddress.port_);
}

}} // namespace sne { namespace server {
