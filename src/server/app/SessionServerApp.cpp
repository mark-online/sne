#include "ServerPCH.h"
#include <sne/server/app/SessionServerApp.h>
#include <sne/base/io_context/IoContext.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/server/session/ServerSideSessionManager.h>
#include <sne/server/rcon/Service.h>
#include <sne/server/config/ConfigReader.h>
#include <sne/base/concurrent/TaskScheduler.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace server {

SessionServerApp::SessionServerApp() :
    callback_(nullptr),
    isWarmedUp_(false)
{
}


SessionServerApp::~SessionServerApp()
{
}


base::IoContextTask& SessionServerApp::getIoContextTask(const std::string& ioContextName)
{
    const auto pos = ioContextTaskMap_.find(ioContextName);
    if (pos == ioContextTaskMap_.end()) {
        throw std::runtime_error(
            core::formatString("IoContextTask(%s) NOT FOUND", ioContextName.c_str()));
    }
    return *((*pos).second);
}


bool SessionServerApp::initDefaultIoContextTask()
{
    return addIoContextTask(SessionServerApp::getDefaultIoContextName(),
        getServerSpec().workerThreadCount_) != nullptr;
}


bool SessionServerApp::initDefaultSessionManager()
{
    if (! shouldUseDefaultSessionManager()) {
        return true;
    }

    const ServerSpec& serverSpec = getServerSpec();
    if (! serverSpec.isValid()) {
        return true;
    }

    sessionManager_.reset(
        new ServerSideSessionManager(getServiceConfig().getServiceName(),
            serverSpec, getIoContextTask(), *this, callback_, this));
    return sessionManager_->initialize();
}


bool SessionServerApp::initTaskScheduler()
{
    taskScheduler_ = std::make_unique<base::TaskScheduler>(getIoContextTask());
    base::TaskScheduler::instance(taskScheduler_.get());
    return true;
}


bool SessionServerApp::initRconService()
{
    const std::string address = getServerSpec().monitorAddress_;
    const uint16_t port = getServerSpec().monitorPort_;
    if (port <= 0) {
        return true;
    }

    rconService_ = std::make_unique<rcon::Service>();
    if (!rconService_->start(address, port)) {
        return false;
    }
    rcon::Service::instance(rconService_.get());

    registerRconQuerySource("session.count", *this);
    registerRconQuerySource("session.total.count", *this);
    return true;
}


void SessionServerApp::registerRconQuerySource(const std::string& name,
    rcon::QuerySource& counterSource)
{
    if (rconService_) {
        rconService_->registerQuerySource(name, counterSource);
    }
}


void SessionServerApp::registerRconCommand(const std::string& name, rcon::Command& command)
{
    if (rconService_) {
        rconService_->registerCommand(name, command);
    }
}


base::IoContextTask* SessionServerApp::addIoContextTask(const std::string& ioContextName,
    uint16_t threadCount)
{
    if (threadCount <= 0) {
        return nullptr;
    }

    SNE_ASSERT(ioContextTaskMap_.find(ioContextName) == ioContextTaskMap_.end());

    const std::string threadName = ioContextName + " I/O thread";

    IoContextTaskPtr ioContextTask(
        std::make_shared<base::IoContextTask>(threadName));
    if (! ioContextTask->start(threadCount)) {
        SNE_LOG_ERROR("Failed to start IoContextTask(%s,%d).",
            ioContextName.c_str(), threadCount);
        return nullptr;
    }

    ioContextTaskMap_.emplace(ioContextName, ioContextTask);
    return ioContextTask.get();
}

// = rcon::QuerySource overriding

std::string SessionServerApp::query(const std::string& name, const std::string& /*detail*/) const
{
    if (sessionManager_.get() != nullptr) {
        if ("session.count" == name) {
            return std::to_string(sessionManager_->getActiveSessionCount());
        }

        if ("session.total.count" == name) {
            return std::to_string(sessionManager_->getTotalSessionCount());
        }
    }
    return "";
}

// = Server overriding

bool SessionServerApp::onInit()
{
    if (!ServerApp::onInit()) {
        return false;
    }

    if (! initDefaultIoContextTask()) {
        SNE_LOG_ERROR("SessionServer::initDefaultIoContextTask() FAILED!");
        return false;
    }

    if (! initDefaultSessionManager()) {
        SNE_LOG_ERROR("SessionServer::initDefaultSessionManager() FAILED!");
        return false;
    }

    if (!initTaskScheduler()) {
        SNE_LOG_ERROR("SessionServer::initTaskScheduler() FAILED!");
        return false;
    }

    if (!initRconService()) {
        SNE_LOG_ERROR("SessionServer::initRconService() FAILED!");
        return false;
    }

    return true;
}


bool SessionServerApp::onStart()
{
    isWarmedUp_ = true;

    if (sessionManager_.get() != nullptr) {
        return sessionManager_->startSessionAcceptor();
    }

    return true;
}


void SessionServerApp::onFinish()
{
    if (sessionManager_.get() != nullptr) {
        sessionManager_->finalize();
    }

    for (auto entry : ioContextTaskMap_) {
        (entry.second)->stop();
    }
}

}} // namespace sne { namespace server {
