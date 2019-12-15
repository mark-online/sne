#pragma once

#include "ServerSideProxy.h"
#include <sne/Common.h>

namespace sne { namespace server {

/**
 * @class ManagedServerSideProxy
 * 관리되는 Server-side proxy.
 * - 서버에서 broadcast 용도로 사용하기 위해 관리함
 * - ServerSideProxyManager도 반드시 인스턴스화해야 한다.
 */
class SNE_API ManagedServerSideProxy : public ServerSideProxy
{
public:
    ManagedServerSideProxy(ServerSideBridge* bridge) :
        ServerSideProxy(bridge) {}

protected:
    /// 더이상 관리하지 않는다
    void unmanage();

private:
    void managed();
    void unmanaged();

protected:
    virtual void bridged() override {
        managed();
    }

    virtual void unbridged() override {
        unmanaged();
    }
};

}} // namespace sne { namespace server {
