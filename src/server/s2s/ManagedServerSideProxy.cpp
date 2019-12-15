#include "ServerPCH.h"
#include <sne/server/s2s/ManagedServerSideProxy.h>
#include <sne/server/s2s/ServerSideProxyManager.h>

// ServerSideProxyManager에 역으로 의존하는 관계가 좋은 디자인은 아니지만,
// 전체적인 디자인에 있어 인터페이스를 통해 우회하는 방법을 사용할 수 없으므로
// 어쩔 수 없이 사용함

namespace sne { namespace server {

void ManagedServerSideProxy::unmanage()
{
    unmanaged();
}


void ManagedServerSideProxy::managed()
{
    SERVERSIDEPROXY_MANAGER->add(getServerId(), this);
}


void ManagedServerSideProxy::unmanaged()
{
    SERVERSIDEPROXY_MANAGER->remove(getServerId());
}

}} // namespace sne { namespace server {
