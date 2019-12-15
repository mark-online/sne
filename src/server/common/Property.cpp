#include "ServerPCH.h"
#include <sne/server/common/Property.h>

namespace sne { namespace server {

namespace
{

Properties s_properties;

} // namespace


void Properties::initialize(const Properties& properties)
{
    s_properties = properties;
}


const Properties& Properties::instance()
{
    return s_properties;
}

}} // namespace sne { namespace server {
