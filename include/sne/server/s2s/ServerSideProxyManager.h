#pragma once

#include "ServerSideProxy.h"
#include <sne/core/utility/Singleton.h>
#include <sne/core/container/Containers.h>
#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#include <mutex>

namespace sne { namespace server {

class ServerSideProxy;

/**
 * @class ServerSideProxyManager
 *
 * ManagedServerSideProxy 관리자
 */
class SNE_API ServerSideProxyManager : public boost::noncopyable
{
    SNE_DECLARE_SINGLETON(ServerSideProxyManager);

public:
    typedef core::HashMap<ServerId, ServerSideProxy*> Proxies;

public:
    ServerSideProxyManager();

    void add(ServerId proxyId, ServerSideProxy* proxy);
    void remove(ServerId proxyId);

    size_t getProxyCount() const;

protected:
    /// proxy 인스턴스는 메모리 풀에서 관리하므로 proxies_의 동기화가
    // 깨지더라도 치명적인 문제가 발생하지는 않는다
    Proxies getCopiedProxies() const;

    ServerSideProxy* getProxy(ServerId proxyId);

private:
    /// Proxy가 삭제되었다
    virtual void proxyRemoved(ServerId proxyId) = 0;

private:
    mutable std::mutex lock_;
    Proxies proxies_;
};

}} // namespace sne { namespace server {

#define SERVERSIDEPROXY_MANAGER \
    sne::server::ServerSideProxyManager::instance()
