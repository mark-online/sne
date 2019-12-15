#include "ServerPCH.h"
#include <sne/server/config/ServerConfig.h>
#include <sne/server/config/ConfigReaderFactory.h>
#include <sne/server/config/ConfigReader.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/Assert.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace
{

const char config_dir[] = "conf";
const char log_dir[] = "log";
const char defaultConfigReader[] = "ini";

} // namespace

namespace sne { namespace server {

ServerConfig::ServerConfig() :
    configReaderType_(defaultConfigReader)
{
}


ServerConfig::~ServerConfig()
{
}


bool ServerConfig::parse(const std::vector<std::string>& args)
{
    return readEnvironmentVariable() &&
        parseCommandLine(args) &&
        parseConfigFile();
}


bool ServerConfig::readEnvironmentVariable()
{
    homeDir_ = boost::filesystem::current_path().string();

    return true;
}


bool ServerConfig::parseCommandLine(const std::vector<std::string>& args)
{
    namespace opt = boost::program_options;

    opt::options_description desc("All options");
    desc.add_options()
        ("help", "show this message")
        ("home,h", opt::value<std::string>(&homeDir_)->default_value(homeDir_),
            "home directory")
        ("config,c", opt::value<std::string>(&configFile_), "config. file path")
        ("format,f",
            opt::value<std::string>(&configReaderType_)->default_value(defaultConfigReader),
            "config. file format");

    try {
        opt::variables_map vm;
        opt::store(opt::command_line_parser(args).options(desc).run(), vm);
        opt::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return false;
        }
    }
    catch (const std::exception& e) {
        SNE_LOG_ERROR("Failed to parse command line(%s)!", e.what());
        return false;
    }

    if (configFile_.empty()) {
        SNE_LOG_ERROR("Config file(-c option) is not specified.");
        return false;
    }

    if (configReaderType_.empty()) {
        SNE_LOG_ERROR(
            "ConfigReader Type(-r option) is not specified.");
        return false;
    }

    homeDir_ = boost::filesystem::canonical(homeDir_).string();

    return true;
}


bool ServerConfig::parseConfigFile()
{
    SNE_ASSERT(! configFile_.empty());

    configReader_.reset(
        ConfigReaderFactory::create(configReaderType_).release());
    if (! configReader_) {
        SNE_LOG_ERROR("Invalid ConfigReaderType(%s)",
            configReaderType_.c_str());
        return false;
    }

    std::string configPath = getConfigPath();
    if (! configReader_->open(configPath)) {
        return false;
    }

    serviceName_ = configReader_->getString("service", "name");

    return true;
}


std::string ServerConfig::getConfigPath() const
{
    boost::filesystem::path path(homeDir_);
    return (path / config_dir / configFile_).string();
}


std::string ServerConfig::getLogDir() const
{
    boost::filesystem::path path(homeDir_);
    return (path / log_dir).string();
}

}} // namespace sne { namespace server {
