#include "ServerPCH.h"
#include <sne/server/s2s/ServerSideProxyManager.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace server {

SNE_DEFINE_SINGLETON(ServerSideProxyManager);

ServerSideProxyManager::ServerSideProxyManager()
{
}


void ServerSideProxyManager::add(ServerId proxyId, ServerSideProxy* proxy)
{
    SNE_ASSERT(isValid(proxyId));
    SNE_ASSERT(proxy != nullptr);

    std::lock_guard<std::mutex> lock(lock_);

    if (proxies_.find(proxyId) != proxies_.end()) {
        SNE_LOG_ERROR("S2S: ServerSideProxyManager::add(S%u) - already exists",
            proxyId);
        proxies_[proxyId] = proxy;
    }
    else {
        proxies_.emplace(proxyId, proxy);
    }

    SNE_LOG_INFO("S2S: ServerSideProxy(S%u) added.", proxyId);
}


void ServerSideProxyManager::remove(ServerId proxyId)
{
    if (! isValid(proxyId)) {
        return;
    }

    std::lock_guard<std::mutex> lock(lock_);

    proxies_.erase(proxyId);

    SNE_LOG_INFO("S2S: ServerSideProxy(#%u) removed.", proxyId);

    proxyRemoved(proxyId);
}


size_t ServerSideProxyManager::getProxyCount() const
{
    std::lock_guard<std::mutex> lock(lock_);

    return proxies_.size();
}


ServerSideProxyManager::Proxies
    ServerSideProxyManager::getCopiedProxies() const
{
    std::lock_guard<std::mutex> lock(lock_);

    ServerSideProxyManager::Proxies clone = proxies_;
    return clone;
}


ServerSideProxy* ServerSideProxyManager::getProxy(ServerId proxyId)
{
    std::lock_guard<std::mutex> lock(lock_);

    const Proxies::iterator pos = proxies_.find(proxyId);
    if (pos == proxies_.end()) {
        return nullptr;
    }

    return (*pos).second;
}

}} // namespace sne { namespace server {
