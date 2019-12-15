#include "BasePCH.h"
#include <sne/base/io_context/IoContextAcceptor.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/session/manager/SessionCreator.h>
#include <sne/base/session/manager/SessionDestroyer.h>
#include <sne/base/session/manager/SessionValidator.h>
#include <sne/base/session/impl/detail/TcpSessionImpl.h>
#include <sne/base/session/Session.h>
#include <sne/base/utility/SystemUtil.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace base {

// = IoContextAcceptor

IoContextAcceptor::IoContextAcceptor(IoContextTask& ioContextTask,
    SessionCreator& sessionCreator,
    SessionDestroyer& sessionDestroyer,
    SessionValidator* validator) :
    ioContextTask_(ioContextTask),
    sessionCreator_(sessionCreator),
    sessionDestroyer_(sessionDestroyer),
    validator_(validator),
    shouldFinish_(false)
{
}


bool IoContextAcceptor::open(const InetAddress& listenAddr, int backlog)
{
    using namespace boost::asio;

    if (shouldFinish_) {
        return false;
    }

    ip::tcp::resolver resolver(ioContextTask_.getIoContext());
    ip::tcp::resolver::query query(listenAddr.address_,
        std::to_string(listenAddr.port_));
    boost::system::error_code ec;
    auto posEp = resolver.resolve(query, ec);
    if (ec) {
        SNE_LOG_DEBUG("IoContextAcceptor::open() failed - resolve(%d,%s)",
            ec.value(), ec.message().c_str());
        return false;
    }

    for (; posEp != ip::tcp::resolver::iterator(); ++posEp) {
        ip::tcp::endpoint endpoint = *posEp;

        auto acceptor = ip::tcp::acceptor(ioContextTask_.getIoContext());
        try {
            acceptor.open(endpoint.protocol());
            acceptor.set_option(ip::tcp::acceptor::reuse_address(true));

            acceptor.bind(endpoint);
            acceptor.listen(backlog == -1 ? socket_base::max_connections : backlog);

            asioAcceptors_.push_back(std::move(acceptor));
        }
        catch (const boost::system::system_error& e) {
            SNE_LOG_DEBUG("IoContextAcceptor::open() failed(%d,%s)",
                e.code().value(), e.what());
            return false;
        }
    }

    if (asioAcceptors_.empty()) {
        return false;
    }

    for (auto& acceptor : asioAcceptors_) {
        accept(acceptor);
    }
    return true;
}


void IoContextAcceptor::close()
{
    shouldFinish_ = true;

    for (auto &acceptor : asioAcceptors_) {
        acceptor.close();
    }
}


void IoContextAcceptor::accept(boost::asio::ip::tcp::acceptor& acceptor)
{
    Session* session = sessionCreator_.acquire();
    assert(session != nullptr);
    auto& sessionImpl = static_cast<TcpSessionImpl&>(session->getImpl());
    auto& socket = sessionImpl.getSocket();

    acceptor.async_accept(socket,
        [this, &acceptor, session, &socket](const boost::system::error_code &error) {
        bool isSucceeded = false;
        if (!error) {
            if (!shouldFinish_) {
                const auto remoteEp = socket.remote_endpoint();
                if ((validator_ != nullptr) && (!validator_->isValidated())) {
                    SNE_LOG_DEBUG("Blocked connection from %s:%d.",
                        remoteEp.address().to_string().c_str(), remoteEp.port());
                    session->disconnect();
                }
                else {
                    SNE_LOG_DEBUG("Connecting from %s:%d...",
                        remoteEp.address().to_string().c_str(), remoteEp.port());
                    session->initialize();
                    sessionCreator_.accepted(*session);
                    isSucceeded = true;
                }
            }
        }
        else {
            if (error != boost::asio::error::operation_aborted) {
                SNE_LOG_DEBUG("IoContextAcceptor error (%d,%s)",
                    error.value(), error.message().c_str());
            }
        }

        if (!isSucceeded) {
            sessionDestroyer_.release(session);
        }

        if (!shouldFinish_) {
            accept(acceptor);
        }
    });
}

}} // namespace sne { namespace base {
