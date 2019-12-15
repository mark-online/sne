#pragma once

#include "config/ServerSideSessionConfig.h"
#include "../common/ServerSpec.h"
#include <sne/base/session/manager/SessionManager.h>
#include <sne/core/container/Containers.h>
#include <sne/Common.h>

namespace sne { namespace base {
class SessionFactory;
class SessionManager;
class SessionManagerCallback;
class IoContextTask;
class IoContextAcceptor;
class SessionValidator;
}} // namespace sne { namespace base {

namespace sne { namespace server {

class SessionFactoryHelper;

/**
 * @class ServerSideSessionManager
 *
 * 서버 측 세션 관리자
 * - (클라이언트) 세션 목록을 유지하고 소켓을 연다
 */
class SNE_API ServerSideSessionManager : public boost::noncopyable
{
    typedef std::unique_ptr<base::IoContextAcceptor> AcceptorPtr;
    typedef std::vector<AcceptorPtr> Acceptors;

public:
    ServerSideSessionManager(const std::string& sessionName,
        const ServerSpec& serverSpec, base::IoContextTask& ioContextTask,
        SessionFactoryHelper& sessionFactoryHelper,
        base::SessionManagerCallback* callback = nullptr,
        base::SessionValidator* validator = nullptr);
    ~ServerSideSessionManager();

    bool initialize();
    void finalize();

    /// socket acceptor를 초기화하고 accept를 시작한다
    bool startSessionAcceptor();

    /// SessionAcceptor를 중지한다
    void stopSessionAcceptor();

    size_t getActiveSessionCount() const;

    size_t getTotalSessionCount() const;

private:
    bool initSessionManager();

    AcceptorPtr createSessionAcceptor(const std::string& address,
        uint16_t port);

private:
    SessionFactoryHelper& sessionFactoryHelper_;
    std::unique_ptr<base::SessionFactory> sessionFactory_;
    std::unique_ptr<base::SessionManager> sessionManager_;
    Acceptors sessionAcceptors_;

    std::string sessionName_;
    ServerSpec serverSpec_;
    base::IoContextTask& ioContextTask_;

    base::SessionManagerCallback* callback_;
    base::SessionValidator* validator_;
};

}} // namespace sne { namespace server {
