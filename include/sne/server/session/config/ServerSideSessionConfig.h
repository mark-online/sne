#pragma once

#include <sne/sgp/session/SgpSessionConfig.h>
#include <sne/sgp/protocol/security/PacketSeedExchangerFactory.h>

namespace sne { namespace server {

/**
 * @struct ServerSideSessionConfig
 *
 * ServerSideSession Config
 */
struct ServerSideSessionConfig : public sgp::SgpSessionConfig
{
    ServerSideSessionConfig() {}

    ServerSideSessionConfig(
        std::unique_ptr<base::MemoryBlockManager> memoryBlockManager,
        std::unique_ptr<sgp::PacketCoder> packetCoder,
        base::SessionDestroyer& sessionDestroyer,
        base::IoContextTask& ioContextTask,
        const base::SessionCapacity& capacity) :
        sgp::SgpSessionConfig(std::move(memoryBlockManager),
            std::move(packetCoder),
            sgp::PacketSeedExchangerFactory::createForServer(),
            sessionDestroyer, ioContextTask, capacity) {}

    bool isValid() const {
        return sgp::SgpSessionConfig::isValid();
    }
};

}} // namespace sne { namespace server {
