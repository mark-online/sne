#include "ServerPCH.h"
#include "ClientSideBridge.h"
#include <sne/server/s2s/ClientSideProxy.h>
#include <sne/sgp/session/SgpSessionImplFactory.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/Assert.h>
#include <sne/core/utility/SystemUtil.h>

namespace sne { namespace server {

ClientSideProxy::ClientSideProxy(const ServerInfo& serverInfo,
    const ClientSideSessionConfig& config) :
    serverInfo_(serverInfo),
    sessionConfig_(config),
    connectionTimeout_(10 * 1000), // TODO: DB Property로 옮길 것
    isConnected_(false)
{
    if (! sessionConfig_.isValid()) {
        return; // for test
    }

    bridge_ = std::make_unique<ClientSideBridge>(*this, sessionConfig_,
        sgp::SgpSessionImplFactory::create(sessionConfig_));
}


ClientSideProxy::~ClientSideProxy()
{
}


bool ClientSideProxy::connect()
{
    if (sessionConfig_.isForTest_) {
        onAuthenticated();
        return true; // 테스트 코드 용
    }

    if (! sessionConfig_.isValid()) {
        return false;
    }

    if (bridge_->bridge(serverInfo_.description_,
        serverInfo_.ip_, serverInfo_.port_, serverInfo_.certificate_)) {
        if (wait(connectionTimeout_)) {
            SNE_LOG_INFO("ClientSideProxy::connect(%u ms) - "
                "Connected to %s(%s:%u) server.",
                connectionTimeout_, serverInfo_.description_.c_str(),
                serverInfo_.ip_.c_str(), serverInfo_.port_);
            return true;
        }
        else {
            SNE_LOG_ERROR("ClientSideProxy::connect(%u ms) - timed out.",
                connectionTimeout_);
        }
    }

    disconnect();

    SNE_LOG_ERROR("ClientSideProxy::connect(%u ms) -"
        " Failed to connect to %s(%s:%u) server.",
        connectionTimeout_, serverInfo_.description_.c_str(),
        serverInfo_.ip_.c_str(), serverInfo_.port_);
    return false;
}


void ClientSideProxy::disconnect()
{
    if (bridge_.get() != nullptr) {
        bridge_->disconnect();
        bridge_->cancelConnection();
    }
}


void ClientSideProxy::sendMessage(base::Message& message)
{
    bridge_->getExtension<base::MessagingExtension>()->sendMessage(message);
}


base::Session* ClientSideProxy::getSession()
{
    return bridge_.get();
}


bool ClientSideProxy::wait(msec_t timeout)
{
    const auto begin = std::chrono::steady_clock::now();

    while (core::getElapsedTime(begin) < timeout) {
        if (isActivated()) {
            return true;
        }
        base::pause(1);
    }
    return false;
}

// = ClientSideProxyCallback overriding

void ClientSideProxy::disconnected()
{
    isConnected_ = false;

    SNE_LOG_INFO("ClientSideProxy::disconnected() - "
        "Disconnected from %s(%s:%u) server.",
        serverInfo_.description_.c_str(), serverInfo_.ip_.c_str(),
        serverInfo_.port_);

    onDisconnected();
}


void ClientSideProxy::onAuthenticate(bool authenticated)
{
    isConnected_ = authenticated;
    if (authenticated) {
        SNE_LOG_DEBUG("ClientSideProxy::s2s_onAuthenticate() - "
            "Authenticated from %s(%s:%u).",
            serverInfo_.description_.c_str(),
            serverInfo_.ip_.c_str(), serverInfo_.port_);
        onAuthenticated();
    }
    else {
        SNE_LOG_ERROR("ClientSideProxy::s2s_onAuthenticate() - "
            "Authentication refused from %s(%s:%u).",
            serverInfo_.description_.c_str(),
            serverInfo_.ip_.c_str(), serverInfo_.port_);
    }
}

}} // namespace sne { namespace server {
