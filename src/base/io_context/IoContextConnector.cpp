#include "BasePCH.h"
#include <sne/base/io_context/IoContextConnector.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/session/manager/SessionCreator.h>
#include <sne/base/session/manager/SessionDestroyer.h>
#include <sne/base/session/impl/detail/TcpSessionImpl.h>
#include <sne/base/session/Session.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/SystemUtil.h>

namespace sne { namespace base {

IoContextConnector::IoContextConnector(IoContextTask& ioContextTask,
    SessionCreator& sessionCreator, SessionDestroyer& sessionDestroyer) :
    ioContextTask_(ioContextTask),
    sessionCreator_(sessionCreator),
    sessionDestroyer_(sessionDestroyer),
    shouldFinish_(false),
    connectionCount_(0)
{
}


bool IoContextConnector::open(const InetAddress& serverAddr,
    size_t connectionCount)
{
    using namespace boost::asio;

    assert(connectionCount > 0);
    connectionCount_ = connectionCount;

    if (shouldFinish_) {
        return false;
    }

    try {
        // TODO: async resolve
        ip::tcp::resolver resolver(ioContextTask_.getIoContext());
        ip::tcp::resolver::query query(serverAddr.address_,
            std::to_string(serverAddr.port_));
        endpoints_ = resolver.resolve(query);
    }
    catch (const boost::system::system_error& e) {
        SNE_LOG_DEBUG("IoContextConnector::open() failed(%d,%s)",
            e.code().value(), e.what());
        return false;
    }

    connect(0);
    return true;
}


void IoContextConnector::close()
{
    shouldFinish_ = true;

    pause(10);
}


void IoContextConnector::connect(size_t connectionIndex)
{
    using namespace boost::asio;

    Session* session = sessionCreator_.acquire();
    assert(session != nullptr);
    auto& sessionImpl = static_cast<TcpSessionImpl&>(session->getImpl());

    boost::asio::async_connect(sessionImpl.getSocket(), endpoints_,
        [this, session, connectionIndex](const boost::system::error_code& error,
            const ip::tcp::endpoint& ep) {
        bool isSucceeded = false;
        if (!error) {
            SNE_LOG_INFO("IoContextConnector::connect() connected(%s:%d)",
                ep.address().to_string().c_str(), ep.port());
            if (!shouldFinish_) {
                session->initialize();
                isSucceeded = true;
            }
        }
        else {
            if (error != boost::asio::error::operation_aborted) {
                SNE_LOG_ERROR("IoContextConnector::connect() failed(%d,%s)",
                    error.value(), error.message().c_str());
                session->disconnect();
            }
        }

        if (!isSucceeded) {
            sessionDestroyer_.release(session);
        }

        if (!shouldFinish_) {
            if ((connectionIndex + 1) < connectionCount_) {
                connect(connectionIndex + 1);
            }
        }
    });
}

}} // namespace sne { namespace base {
