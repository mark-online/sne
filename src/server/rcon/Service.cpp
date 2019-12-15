#include "ServerPCH.h"
#include <sne/server/rcon/Service.h>
#include <sne/server/rcon/Command.h>
#include <sne/server/rcon/QuerySource.h>
#include <sne/base/concurrent/TaskScheduler.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/utility/Win32Util.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4100 4267)
#  undef NOMINMAX
#endif
#include <anyrpc/api.h>
#include <anyrpc/error.h>
#include <anyrpc/logger.h>
#include <anyrpc/value.h>
#include <anyrpc/method.h>
#include <anyrpc/stream.h>
#include <anyrpc/socket.h>
#include <anyrpc/connection.h>
#include <anyrpc/server.h>
#include <anyrpc/json/jsonserver.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/core/ignore_unused.hpp>

namespace sne { namespace server { namespace rcon {

namespace detail {

    /**
    * @class QueryRpcMethod
    * 서버 정보 쿼리 용 RPC method
    */
    class QueryRpcMethod : public anyrpc::Method
    {
        typedef std::map<std::string, QuerySource*> QuerySourceMap;

    public:
        QueryRpcMethod() :
            anyrpc::Method("query", "query the server information", false) {}

        void addQuerySource(const std::string& name, QuerySource& querySource) {
            assert(m_querySourceMap.find(name) == m_querySourceMap.end() &&
                "query source name is duplicated");
            m_querySourceMap.emplace(name, &querySource);
        }

    private:
        virtual void Execute(anyrpc::Value& params, anyrpc::Value& result) override {
            if ((!params.IsArray()) || (params.Size() != 2)) {
                result["result"] = R"({"error":"invalid paramter"})";
                return;
            }
            const anyrpc::Value& param1 = const_cast<const anyrpc::Value&>(params[0]);
            const anyrpc::Value& param2 = const_cast<const anyrpc::Value&>(params[1]);

            const std::string name = param1.GetString();
            const std::string detail = param2.GetString();
            const QuerySourceMap::const_iterator pos = m_querySourceMap.find(name);
            if (pos == m_querySourceMap.end()) {
                result["result"] = R"({"error":"can't find query source"})";
                return;
            }

            QuerySource* querySource = (*pos).second;
            //result["result"] = querySource->query(name, detail);
            result = querySource->query(name, detail);
        }

    private:
        QuerySourceMap m_querySourceMap;
    };


    /**
    * @class CommandRpcMethod
    * 서버에 명령 처리 의뢰 용 RPC method
    */
    class CommandRpcMethod : public anyrpc::Method
    {
        typedef std::map<std::string, Command*> CommandMap;

    public:
        CommandRpcMethod() :
            anyrpc::Method("command", "execute a command", false) {}

        void addCommand(const std::string& name, Command& command) {
            assert(m_commandMap.find(name) == m_commandMap.end() &&
                "command name is duplicated");
            m_commandMap.emplace(name, &command);
        }

    public:
        virtual void Execute(anyrpc::Value& params, anyrpc::Value& result) override {
            if ((!params.IsArray()) || (params.Size() < 2)) {
                result["result"] = R"({"error":"invalid paramter"})";
                return;
            }
            const std::string name = params[0].GetString();

            const CommandMap::const_iterator pos = m_commandMap.find(name);
            if (pos == m_commandMap.end()) {
                result["result"] = R"({"error":"can't find command"})";
                return;
            }

            Command* command = (*pos).second;
            anyrpc::Value& param1 = params[1];
            anyrpc::Value& param2 = params[2];
            anyrpc::Value& param3 = params[3];
            //result["result"] = command->execute(name, param1, param2, param3);
            result = command->execute(name, param1, param2, param3);
        }

    private:
        CommandMap m_commandMap;
    };


    /// JSON-RPC 테스트용 RPC
    class EchoQuerySource : public QuerySource
    {
    public:
        static EchoQuerySource& getInstance() {
            static EchoQuerySource instance;
            return instance;
        }
    private:
        virtual std::string query(const std::string& name,
            const std::string& detail) const override {
            assert(name == "echo" && "precondition");
            boost::ignore_unused(name);
            return detail;
        }
    };

} // namespace detail

// = Service

SNE_DEFINE_SINGLETON(Service);

Service::Service() :
    isRunning_(false)
{
    rpcServer_ = std::make_unique<anyrpc::JsonHttpServer>();
    anyrpc::MethodManager* methodManager = rpcServer_->GetMethodManager();

    queryRpc_ = std::make_unique<detail::QueryRpcMethod>();
    methodManager->AddMethod(queryRpc_.get());

    commandRpc_ = std::make_unique<detail::CommandRpcMethod>();
    methodManager->AddMethod(commandRpc_.get());

#ifndef NDEBUG
    registerQuerySource("echo", detail::EchoQuerySource::getInstance());
#endif
}


Service::~Service()
{
    stop();
}


bool Service::start(const std::string& address, uint16_t port)
{
    boost::ignore_unused(address);

    if (!rpcServer_->BindAndListen(port)) {
        return false;
    }

    isRunning_ = true;

    const msec_t interval = 10;
    TASK_SCHEDULER->schedule(std::make_unique<sne::base::LambdaTask>([&]() {
        if (isRunning_) {
            std::lock_guard<std::mutex> lock(lock_);
            rpcServer_->Work(5);
        }
    }), 0, interval);

    SNE_LOG_INFO("Rcon Service started(%s:%d)", address.c_str(), port);
    return true;
}


void Service::stop()
{
    if (!isRunning_) {
        return;
    }

    isRunning_ = false;

    if (rpcServer_.get() != NULL) {
        rpcServer_->Shutdown();
    }
}


void Service::registerQuerySource(const std::string& name, QuerySource& querySource)
{
    std::lock_guard<std::mutex> lock(lock_);

    queryRpc_->addQuerySource(name, querySource);
}


void Service::registerCommand(const std::string& name, Command& command)
{
    std::lock_guard<std::mutex> lock(lock_);

    commandRpc_->addCommand(name, command);
}

}}} // namespace sne { namespace server { namespace rcon {
