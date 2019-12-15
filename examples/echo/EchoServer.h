#pragma once

#include <sne/Common.h>
#include "EchoConfig.h"
#include "Echoer.h"
#include <sne/base/session/manager/SessionFactory.h>
#include <sne/server/session/ServerSideSession.h>
#include <sne/base/memory/MemoryBlockManager.h>
#include <sne/srpc/RpcForwarder.h>
#include <sne/srpc/RpcReceiver.h>

namespace sne { namespace base {
class IoContextTask;
}} // namespace sne { namespace base {

using namespace sne;

/**
 * @class EchoServerSession
 */
class EchoServerSession :
    public server::ServerSideSession,
    public Echo,
    protected srpc::RpcForwarder, protected srpc::RpcReceiver
{
    DECLARE_SRPC_EVENT_DISPATCHER(EchoServerSession);
public:
    EchoServerSession(bool useRpc,
        const server::ServerSideSessionConfig& sessionConfig,
        const server::ServerSpec& serverSpec,
        std::unique_ptr<base::SessionImpl> impl);

public:
    OVERRIDE_SRPC_METHOD_1(echo, std::string, data);
    OVERRIDE_SRPC_METHOD_1(onEcho, std::string, data);

private:
    virtual bool onConnected() override;

private:
    void echoMessageReceived(const base::Message& message);

private:
    bool useRpc_;
};


/**
* @class ServerSessionFactory
*/
class ServerSessionFactory : public base::SessionFactory
{
public:
    ServerSessionFactory(const EchoConfig& config,
        base::IoContextTask& ioContextTask) :
        config_(config),
        ioContextTask_(ioContextTask),
        sessionDestroyer_(nullptr) {}

    virtual void setSessionDestroyer(
        base::SessionDestroyer& sessionDestroyer) override {
        sessionDestroyer_ = &sessionDestroyer;
    }

    virtual std::unique_ptr<base::Session> create() const override;

private:
    const EchoConfig& config_;
    base::IoContextTask& ioContextTask_;
    base::SessionDestroyer* sessionDestroyer_;
};
