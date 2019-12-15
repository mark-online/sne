#pragma once

#include <sne/Common.h>
#include <memory>

namespace sne { namespace server {

class ConfigReader;

/**
 * @class ConfigReaderFactory
 *
 * ConfigReader Factory
 */
struct SNE_API ConfigReaderFactory
{
    /// @param readerType "ini" only
    static std::unique_ptr<ConfigReader> create(const std::string& readerType);
};

}} // namespace sne { namespace server {
