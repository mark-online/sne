#include "ServerPCH.h"
#include <sne/server/session/ServerSideSessionManager.h>
#include <sne/server/session/SessionFactoryHelper.h>
#include <sne/base/io_context/IoContextAcceptor.h>
#include <sne/base/session/manager/CachedSessionManager.h>
#include <sne/base/session/manager/SessionFactory.h>
#include <sne/server/utility/Profiler.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/SystemUtil.h>

namespace sne { namespace server {

ServerSideSessionManager::ServerSideSessionManager(
    const std::string& sessionName,
    const ServerSpec& serverSpec, base::IoContextTask& ioContextTask,
    SessionFactoryHelper& sessionFactoryHelper,
    base::SessionManagerCallback* callback,
    base::SessionValidator* validator) :
    sessionFactoryHelper_(sessionFactoryHelper),
    sessionName_(sessionName),
    serverSpec_(serverSpec),
    ioContextTask_(ioContextTask),
    callback_(callback),
    validator_(validator)
{
}


ServerSideSessionManager::~ServerSideSessionManager()
{
}


bool ServerSideSessionManager::initialize()
{
    BaseProfiler<> profiler(__FUNCTION__);

    SNE_LOG_DEBUG("ServerSideSessionManager::initialize(%s)",
        sessionName_.c_str());

    return initSessionManager();
}


void ServerSideSessionManager::finalize()
{
    BaseProfiler<> profiler(__FUNCTION__);

    SNE_LOG_DEBUG("ServerSideSessionManager::finalize(%s)",
        sessionName_.c_str());

    stopSessionAcceptor();

    if (sessionManager_) {
        sessionManager_->cancel();
        sessionManager_->waitForDone();
    }
}


bool ServerSideSessionManager::startSessionAcceptor()
{
    sessionAcceptors_.clear();

    for (const std::string& address : serverSpec_.listeningAddresses_) {
        if (address.empty()) {
            continue;
        }

        AcceptorPtr acceptor(
            createSessionAcceptor(address, serverSpec_.listeningPort_));
        if (! acceptor.get()) {
            SNE_LOG_ERROR("ServerSideSessionManager::startSessionAcceptor() FAILED.");
            return false;
        }

        sessionAcceptors_.push_back(std::move(acceptor));
    }

    return true;
}


void ServerSideSessionManager::stopSessionAcceptor()
{
    for (AcceptorPtr& acceptor : sessionAcceptors_) {
        acceptor->close();
    }
}


size_t ServerSideSessionManager::getActiveSessionCount() const
{
    assert(sessionManager_.get() != nullptr);

    return sessionManager_->getSessionCount();
}


size_t ServerSideSessionManager::getTotalSessionCount() const
{
    assert(sessionManager_.get() != nullptr);

    return sessionManager_->getTotalSessionCount();
}


bool ServerSideSessionManager::initSessionManager()
{
    SNE_ASSERT(! sessionFactory_);
    sessionFactory_.reset(
        sessionFactoryHelper_.createSessionFactory(serverSpec_, ioContextTask_).
            release());

    sessionManager_ =
        std::make_unique<base::CachedSessionManager>(sessionName_,
            serverSpec_.sessionPoolSize_, *sessionFactory_, callback_);
    sessionManager_->initialize();
    return true;
}


ServerSideSessionManager::AcceptorPtr
ServerSideSessionManager::createSessionAcceptor(
    const std::string& address, uint16_t port)
{
    const base::InetAddress listenAddress(port, address);

    AcceptorPtr newAcceptor = std::make_unique<base::IoContextAcceptor>(
        ioContextTask_, *sessionManager_, *sessionManager_, validator_);
    if (! newAcceptor->open(listenAddress)) {
        SNE_LOG_ERROR(
            "ServerSideSessionManager::createSessionAcceptor(%s:%d) FAILED!",
            address.c_str(), port);
        return AcceptorPtr();
    }

    SNE_LOG_INFO("ProactorSessionAcceptor started(%s:%d)",
        address.c_str(), port);

    return newAcceptor;
}

}} // namespace sne { namespace server {
