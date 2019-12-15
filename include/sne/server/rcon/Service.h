#pragma once

#include <sne/core/container/Containers.h>
#include <sne/core/utility/Singleton.h>
#include <sne/Common.h>
#include <mutex>
#include <memory>

namespace anyrpc {
class JsonHttpServer;
} // namespace anyrpc {

namespace sne { namespace server { namespace rcon {

namespace detail {
class QueryRpcMethod;
class CommandRpcMethod;
} // namespace detail

class QuerySource;
class Command;


/**
 * @class Service
 *
 * Montior Service
 */
class SNE_API Service : public boost::noncopyable
{
    SNE_DECLARE_SINGLETON(Service);

public:
    Service();
    ~Service();

    bool start(const std::string& address, uint16_t port);
    void stop();

public:
    void registerQuerySource(const std::string& name, QuerySource& querySource);
    void registerCommand(const std::string& name, Command& command);

private:
    std::unique_ptr<anyrpc::JsonHttpServer> rpcServer_;
    volatile bool isRunning_;

    std::unique_ptr<detail::QueryRpcMethod> queryRpc_;
    std::unique_ptr<detail::CommandRpcMethod> commandRpc_;

    /// 서비스 구동 중에 RPC method를 추가할 수 있도록 하기 위해 도입
    std::mutex lock_;
};

}}} // namespace sne { namespace server { namespace rcon {

#define RCON_SERVICE	sne::server::rcon::Service::instance()