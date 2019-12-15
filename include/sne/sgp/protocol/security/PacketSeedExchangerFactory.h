#pragma once

#include <sne/Common.h>
#include <memory>

namespace sne { namespace sgp {

class PacketSeedExchanger;

/**
 * @struct PacketSeedExchangerFactory
 */
struct SNE_API PacketSeedExchangerFactory
{
    static std::unique_ptr<PacketSeedExchanger> createForServer();

    static std::unique_ptr<PacketSeedExchanger> createForClient();
};

}} // namespace sne { namespace sgp {
