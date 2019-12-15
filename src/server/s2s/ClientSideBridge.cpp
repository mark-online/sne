#include "ServerPCH.h"
#include "ClientSideBridge.h"
#include <sne/server/s2s/ClientSideBridgeCallback.h>
#include <sne/server/s2s/S2sMessage.h>
#include <sne/server/session/config/ClientSideSessionConfig.h>
#include <sne/base/io_context/IoContext.h>
#include <sne/base/io_context/Timer.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>
#include <boost/asio.hpp>

namespace sne { namespace server {

namespace detail
{

/**
 * @class ClientSideBridgeConnector
 * 주기적으로 서버에 접속을 시도한다
 */
class ClientSideBridgeConnector : private base::TimerCallback
{
    enum { initialRetryDelay = 500, maxRetryDelay = 7000 };

public:
    ClientSideBridgeConnector(base::Session& session) :
        session_(session),
        socket_(session.getIoContext()),
        retryDelay_(initialRetryDelay),
        reconnectTimer_(session.getIoContext().createTimer(*this)) {}

    void connect(const std::string& serverName, const std::string& ip, uint16_t port) {
        serverName_ = serverName;
        remoteAddr_.set(port, ip);
        connect();
    }

    void cancel() {
        reconnectTimer_->cancel();

        boost::system::error_code error;
        socket_.close(error);
    }

    void reconnect() {
        retryDelay_ = (std::min)(retryDelay_ * 2, msec_t(maxRetryDelay));
        reconnectTimer_->once(retryDelay_);
        SNE_ASSERT(reconnectTimer_->isRunning());
        SNE_LOG_INFO(
            "S2S: Retrying to connect Server(%s,%s:%d)(delay:%dms)",
            serverName_.c_str(),
            remoteAddr_.address_.c_str(), remoteAddr_.port_, retryDelay_);
    }

private:
    void connect() {
        using namespace boost::asio;

        ip::tcp::resolver resolver(session_.getIoContext());
        ip::tcp::resolver::query query(remoteAddr_.address_,
            std::to_string(remoteAddr_.port_));
        auto endpoints = resolver.resolve(query);

        boost::asio::async_connect(socket_, endpoints,
            [this](const boost::system::error_code& error, const ip::tcp::endpoint& ep) {
            if (error) {
                if (error != boost::asio::error::operation_aborted) {
                    SNE_LOG_ERROR("S2S: Failed to connect(%s:%d) - (%d,%s)",
                        remoteAddr_.address_.c_str(), remoteAddr_.port_,
                        error.value(), error.message().c_str());
                }
                reconnect();
            }
            else {
                retryDelay_ = initialRetryDelay;

                SNE_LOG_INFO("S2S: connected(%s:%d)",
                    ep.address().to_string().c_str(), ep.port());
                session_.initialize();
            }
        });
    }

private:
    // = base::TimerCallback overriding
    virtual void timedOut(const void* /*act*/) override {
        connect();
    }

private:
    base::Session& session_;
    boost::asio::ip::tcp::socket socket_;
    std::string serverName_;
    base::InetAddress remoteAddr_;
    msec_t retryDelay_;
    std::unique_ptr<base::Timer> reconnectTimer_;
};

} // namespace detail

// = ClientSideBridge

ClientSideBridge::ClientSideBridge(ClientSideBridgeCallback& callback,
    const ClientSideSessionConfig& config, std::unique_ptr<base::SessionImpl> impl) :
    ClientSideSession(config, std::move(impl)),
    connector_(std::make_unique<detail::ClientSideBridgeConnector>(*this)),
    callback_(&callback),
    isShuttingDown_(false)
{
    getExtension<base::MessagingExtension>()->
        registerMessageCallback<S2sAuthenticateResultMessage>(
            &ClientSideBridge::authenticateResultMessageReceived, this);
}


ClientSideBridge::~ClientSideBridge()
{
    isShuttingDown_ = true;
    connector_->cancel();
    disconnect();
}


bool ClientSideBridge::bridge(const std::string& serverName,
    const std::string& address, uint16_t port,
    const S2sCertificate& certificate)
{
    certificate_ = certificate;

    connector_->connect(serverName, address, port);

    return true;
}


void ClientSideBridge::cancelConnection()
{
    connector_->cancel();
}


void ClientSideBridge::authenticateResultMessageReceived(
    const base::Message& message)
{
    const S2sAuthenticateResultMessage& theMessage =
        static_cast<const S2sAuthenticateResultMessage&>(message);

    callback_->onAuthenticate(theMessage.authenticated_);
}

// = overriding

void ClientSideBridge::onDisconnected()
{
    ClientSideSession::onDisconnected();

    callback_->disconnected();

    if (! isShuttingDown_) {
        connector_->reconnect();
    }
}

// = sgp::PacketSeedExchangerCallback overriding

void ClientSideBridge::seedExchanged()
{
    S2sAuthenticateMessage message(certificate_);
    (void)getExtension<base::MessagingExtension>()->sendMessage(message);
}

}} // namespace sne { namespace server {
