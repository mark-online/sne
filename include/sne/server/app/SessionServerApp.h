#pragma once

#include "ServerApp.h"
#include "../session/SessionFactoryHelper.h"
#include "../rcon/QuerySource.h"
#include <sne/base/session/manager/SessionValidator.h>
#include <sne/Common.h>
#include <sne/core/container/Containers.h>

namespace sne { namespace base {
class IoContext;
class IoContextTask;
class SessionManagerCallback;
class SessionValidator;
class TaskScheduler;
}} // namespace sne { namespace base {

namespace sne { namespace server {

namespace rcon {
class Service;
class QuerySource;
class Command;
} // namespace rcon


class ServerSideSessionManager;

/**
 * @class SessionServer
 *
 * 세션을 생성하는 서버 기반 클래스
 */
class SNE_API SessionServerApp :
    public ServerApp,
    public SessionFactoryHelper,
    protected rcon::QuerySource,
    private base::SessionValidator
{
    typedef std::shared_ptr<base::IoContextTask> IoContextTaskPtr;
    typedef core::HashMap<std::string, IoContextTaskPtr> IoContextTaskMap;

    typedef std::shared_ptr<base::IoContextTask> IoContextTaskPtr;
    typedef core::Vector<IoContextTaskPtr> IoContextTasks;

public:
    SessionServerApp();
    virtual ~SessionServerApp();

public:
    void registerRconQuerySource(const std::string& name,
        rcon::QuerySource& counterSource);
    void registerRconCommand(const std::string& name, rcon::Command& command);

protected:
    void setSessionManagerCallback(base::SessionManagerCallback& callback) {
        callback_ = &callback;
    }

    base::IoContextTask& getIoContextTask(const std::string& ioContextName =
        SessionServerApp::getDefaultIoContextName());

    ServerSideSessionManager& getServerSideSessionManager() {
        assert(sessionManager_.get() != nullptr);
        return *sessionManager_;
    }

protected: // = Server overriding
    virtual bool onInit() override;
    virtual bool onStart() override;
    virtual void onFinish() override;

protected: // = rcon::QuerySource overriding
    virtual std::string query(const std::string& name,
        const std::string& detail) const override;

protected:
    // = base::SessionValidator overriding
    virtual bool isValidated() const override {
        return isWarmedUp_;
    }

private:
    /// C2S 용 기본 세션 관리자를 사용할 것인가?
    virtual bool shouldUseDefaultSessionManager() const {
        return true;
    }

protected:
    base::IoContextTask* addIoContextTask(const std::string& ioContextName,
        uint16_t threadCount);

private:
    bool initDefaultIoContextTask();
    bool initDefaultSessionManager();
    bool initTaskScheduler();
    bool initRconService();

private:
    static const std::string& getDefaultIoContextName() {
        static const std::string defaultName("default");
        return defaultName;
    }

private:
    IoContextTaskMap ioContextTaskMap_;
    std::unique_ptr<base::TaskScheduler> taskScheduler_;

    std::unique_ptr<rcon::Service> rconService_;
    std::unique_ptr<ServerSideSessionManager> sessionManager_;

    base::SessionManagerCallback* callback_;

    /// 서비스 준비 완료?
    bool isWarmedUp_;
};

}} // namespace sne { namespace server {
