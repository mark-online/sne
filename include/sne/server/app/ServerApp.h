#pragma once

#include "DynamicObject.h"
#include "../config/ServerConfig.h"
#include <sne/Common.h>

namespace sne { namespace base {
class IoContext;
class IoContextTask;
}} // namespace sne { namespace base {

namespace sne { namespace server {

struct ServerSpec;

/**
 * @class Server
 *
 * 서버 기반 클래스
 */
class SNE_API ServerApp : public DynamicObject
{
public:
    static ServerApp* instance();
    static void instance(std::unique_ptr<ServerApp> app);

public:
    ServerApp();
    virtual ~ServerApp();

public: // = DynamicObject overriding
    virtual bool initialize(const std::vector<std::string>& args) override;
    virtual void finalize() override;

protected:
    const ServerConfig& getServiceConfig() const {
        return serviceConfig_;
    }

protected:
    /// 서비스를 초기화한다.
    virtual bool onInit();

    /// 서비스를 시작한다
    virtual bool onStart() = 0;

    /// 서비스를 종료한다(주의!! onInit()이 실패해도 호출된다).
    virtual void onFinish() = 0;

    /// 세션 서비스를 위한 컨픽
    virtual const ServerSpec& getServerSpec() const = 0;

private:
    void initLogger();

    std::string getPidPath() const;

private:
    ServerConfig serviceConfig_;
};

}} // namespace sne { namespace server {


#define SNE_SET_SERVER_APP(ServiceClass) \
    sne::server::ServerApp::instance(std::make_unique<ServiceClass>())
