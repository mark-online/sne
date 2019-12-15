#include "ServerPCH.h"
#ifdef _MSC_VER
#  pragma warning(disable : 4503)
#endif
#include "IniFileConfigReader.h"
#include <sne/base/utility/Logger.h>
#include <boost/property_tree/ini_parser.hpp>

namespace sne { namespace server {

bool IniFileConfigReader::open(const std::string& filename)
{
    try {
        boost::property_tree::ini_parser::read_ini(filename, ptree_);
    }
    catch (const std::exception& e) {
        SNE_LOG_ERROR("Failed to read INI file(%s,%s)",
            filename.c_str(), e.what());
        return false;
    }
    return true;
}


std::string IniFileConfigReader::getString(const char* sectionName,
    const char* keyName)
{
    std::ostringstream oss;
    oss << sectionName << "." << keyName;
    std::string value;
    try {
        value = ptree_.get<std::string>(oss.str());
    }
    catch (const std::exception& /*e*/) {
        throw NotDefinedConfigError(__FILE__, __LINE__, sectionName, keyName);
    }
    return value;
}

}} // namespace sne { namespace server {
