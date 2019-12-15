#include "ServerPCH.h"
#include <sne/server/config/ConfigReaderFactory.h>
#ifdef _MSC_VER
#  pragma warning(disable : 4503)
#endif
#include "IniFileConfigReader.h"

namespace sne { namespace server {

std::unique_ptr<ConfigReader> ConfigReaderFactory::create(
    const std::string& readerType)
{
    if (readerType == "ini") {
        return std::make_unique<IniFileConfigReader>();
    }
    else {
        assert(false && "not supported!");
    }
    return std::unique_ptr<ConfigReader>();
}

}} // namespace sne { namespace server {
