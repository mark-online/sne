#pragma once

#include <sne/srpc/RpcContainers.h>
#include <boost/lexical_cast.hpp>
#include <string>

namespace sne { namespace server {

/**
 * @class Properties
 */
class SNE_API Properties : public srpc::RMap<std::string, std::string>
{
public:
    static void initialize(const Properties& properties);

    static const Properties& instance();

public:
    template <typename T>
    static T getProperty(const std::string& key) /*throw (boost::bad_lexical_cast)*/
    {
        const Properties::const_iterator pos = instance().find(key);
        if (pos != instance().end()) {
            return boost::lexical_cast<T>((*pos).second);
        }
        return T();
    }
};

}} // namespace sne { namespace server {

#define SNE_PROPERTIES sne::server::Properties
