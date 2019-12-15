#pragma once

#include <sne/server/config/ConfigReader.h>
#include <boost/property_tree/ptree.hpp>

namespace sne { namespace server {

/**
 * @class IniFileConfigReader
 *
 * INI 파일 reader
 */
class IniFileConfigReader : public ConfigReader
{
public:
    virtual bool open(const std::string& filename) override;

    virtual std::string getString(const char* sectionName,
        const char* keyName) override;
private:
    boost::property_tree::ptree ptree_;
};

}} // namespace sne { namespace server {
