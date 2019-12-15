#include "SgpPCH.h"
#include "PacketSeedExchangerImpl.h"
#include <sne/sgp/protocol/security/PacketSeedExchangerFactory.h>

namespace sne { namespace sgp {

std::unique_ptr<PacketSeedExchanger> PacketSeedExchangerFactory::createForServer()
{
    return std::make_unique<PacketSeedExchangerForServer>();
}


std::unique_ptr<PacketSeedExchanger> PacketSeedExchangerFactory::createForClient()
{
    return std::make_unique<PacketSeedExchangerForClient>();
}

}} // namespace sne { namespace sgp {
