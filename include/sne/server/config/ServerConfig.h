#pragma once

#include <sne/Common.h>
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>

namespace sne { namespace server {

class ConfigReader;

/**
 * @class ServerConfig
 *
 * 서버 설정
 * - 각 섹션의 값을 가져올 때마다(getProperty()) 메모리 검색을 하므로 자주
 *   사용하는 값은 캐싱하는 것이 좋다.
 */
class SNE_API ServerConfig : public boost::noncopyable
{
public:
    ServerConfig();
    virtual ~ServerConfig();

    /// 설정 내역을 파싱한다.
    bool parse(const std::vector<std::string>& args);

    /// 서비스명을 얻어온다
    std::string getServiceName() const {
        return serviceName_;
    }

    /// 홈 디렉토리를 얻는다
    std::string getHomeDirectory() const {
        return homeDir_;
    }

    /// 설정 파일의 전체 경로를 얻는다.
    std::string getConfigPath() const;

    /// 로그 디렉토리를 얻는다.
    std::string getLogDir() const;

    ConfigReader& getConfigReader() const {
        return *configReader_;
    };

private:
    /// 시스템 환경 변수를 읽어온다
    bool readEnvironmentVariable();

    /**
    * command-line을 파싱한다.
    *
    * <Options>
    * - --config or -c <config-file> : 설정 파일 지정
    */
    bool parseCommandLine(const std::vector<std::string>& args);

    /// 설정 파일을 파싱한다.
    bool parseConfigFile();

private:
    std::unique_ptr<ConfigReader> configReader_;
    /// home directory
    std::string homeDir_;
    /// config. file path
    std::string configFile_;
    /// ConfigReader type
    std::string configReaderType_;
    /// 서비스 이름(덤프 파일 등에 사용된다)
    std::string serviceName_;
};

}} // namespace sne { namespace server {
