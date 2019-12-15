#pragma once

#include <sne/Common.h>
#include "EchoConfig.h"
#include "Echoer.h"
#include <sne/base/session/manager/SessionFactory.h>
#include <sne/server/session/ClientSideSession.h>
#include <sne/srpc/RpcForwarder.h>
#include <sne/srpc/RpcReceiver.h>

namespace sne { namespace base {
class IoContextTask;
}} // namespace sne { namespace base {

using namespace sne;

class ClientEchoer;

/**
 * @class EchoClientSession
 */
class EchoClientSession :
    public server::ClientSideSession,
    public Echo,
    protected srpc::RpcForwarder, protected srpc::RpcReceiver
{
    DECLARE_SRPC_EVENT_DISPATCHER(EchoClientSession);
public:
    EchoClientSession(bool useRpc, uint32_t echoCount, uint32_t blockSize,
        const server::ClientSideSessionConfig& config,
        std::unique_ptr<base::SessionImpl> impl);

    void echoed();

public:
    OVERRIDE_SRPC_METHOD_1(echo, std::string, data);
    OVERRIDE_SRPC_METHOD_1(onEcho, std::string, data);

private:
    virtual void onValidated() override;

    virtual bool onConnected() override;

private:
    void echoMessageReceived(const base::Message& message);

    void sendEcho();

private:
    const bool useRpc_;
    const uint32_t echoCount_;
    const std::string body_;
    uint32_t echoedCount_;
};


/**
* @class ClientSessionFactory
*/
class ClientSessionFactory : public base::SessionFactory
{
public:
    ClientSessionFactory(const EchoConfig& config,
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
