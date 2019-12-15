#include "ServerPCH.h"
#include <sne/server/app/ServerApp.h>
#include <sne/server/rcon/Service.h>
#include <sne/server/config/ConfigReader.h>
#include <sne/server/utility/CrashHandler.h>
#include <sne/server/utility/Profiler.h>
#include <sne/server/common/ServerSpec.h>
#include <sne/base/io_context/IoContext.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/utility/LogManager.h>
#include <sne/base/utility/Logger.h>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <boost/filesystem/operations.hpp>

namespace sne { namespace server {

namespace {

/// PID(Process ID) 파일을 작성한다
inline void writePid(const std::string& pidPath)
{
    std::ofstream pidFile(pidPath);
    if (! pidFile) {
        SNE_LOG_ERROR("Can't write to PID file(%s).",
            pidPath.c_str());
    }
    else {
        pidFile << boost::interprocess::ipcdetail::get_current_process_id();
    }
}


/// PID(Process ID) 파일을 삭제한다
inline void removePid(const std::string& pidPath)
{
    boost::system::error_code error;
    boost::filesystem::remove(pidPath, error);
}


std::unique_ptr<ServerApp> s_serverApp;

} // namespace

// = ServerApp


ServerApp* ServerApp::instance()
{
    return s_serverApp.get();
}


void ServerApp::instance(std::unique_ptr<ServerApp> app)
{
    assert(!s_serverApp);

    s_serverApp = std::move(app);
}


ServerApp::ServerApp()
{
}


ServerApp::~ServerApp()
{
}


bool ServerApp::initialize(const std::vector<std::string>& args)
{
    SNE_LOG_INFO("ServerApp::init() starting...");

    {
        Profiler profiler(__FUNCTION__);

        try {
            if (!serviceConfig_.parse(args)) {
                throw std::runtime_error("Failed to parse config.");
            }

#ifdef _WIN32
            setCrtInvalidParameterHandler();
            setPureVirtualFunctionCallHandler();

            setCrashHandler(serviceConfig_.getLogDir(),
                serviceConfig_.getServiceName());
#endif

            initLogger();

            SNE_LOG_INFO("ServerApp::onInit() starting...");
            if (!onInit()) {
                throw std::runtime_error("Failed to ServerApp::onInit().");
            }

            SNE_LOG_INFO("ServerApp::onStart() starting...");
            if (!onStart()) {
                throw std::runtime_error("Failed to ServerApp::onStart().");
            }
        }
        catch (const std::exception& e) {
            SNE_LOG_ERROR("ServerApp::init() failed - %s", e.what());
            onFinish();
            return false;
        }

        writePid(getPidPath());
    }

    SNE_LOG_INFO("%s starting...", getServerSpec().name_.c_str());
    return true;
}


void ServerApp::finalize()
{
    SNE_LOG_INFO("ServerApp::fini() starting...");

    {
        Profiler profiler(__FUNCTION__);

        SNE_LOG_INFO("ServerApp::onFinish() starting...");
        onFinish();

        removePid(getPidPath());
    }

    SNE_LOG_INFO("%s stopped.", getServerSpec().name_.c_str());
}


std::string ServerApp::getPidPath() const
{
    return getServiceConfig().getLogDir() + "/" +
        getServiceConfig().getServiceName() + ".pid";
}


bool ServerApp::onInit()
{
    using namespace boost::filesystem;
    create_directories(getServiceConfig().getLogDir());
    current_path(serviceConfig_.getHomeDirectory());

    return true;
}


void ServerApp::initLogger()
{
    const char sectionLog[] = "log";

    const ServerConfig& serviceConfig = getServiceConfig();
    sne::server::ConfigReader& configReader = serviceConfig.getConfigReader();

    const std::string logFilePath = serviceConfig.getLogDir() + "/" +
        serviceConfig.getServiceName() +
        configReader.getString(sectionLog, "file-pattern");
    const size_t logRotationSize =
        configReader.getNumeric<size_t>(sectionLog, "rotation-size");
    const std::string serverityLevel =
        configReader.getString(sectionLog, "severity-level");

    base::LogManager logManager;
    logManager.setSeverityLevel(serverityLevel);
    logManager.enableFileLog(logFilePath, logRotationSize);
}

}} // namespace sne { namespace server {
