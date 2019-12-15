#pragma once

#include <sne/sgp/session/SgpSessionConfig.h>
#include <sne/sgp/protocol/security/PacketSeedExchangerFactory.h>

namespace sne { namespace server {

/**
 * @struct ClientSideSessionConfig
 */
struct ClientSideSessionConfig : public sgp::SgpSessionConfig
{
    msec_t heartbeatInterval_;

    ClientSideSessionConfig() :
        heartbeatInterval_(0) {}

    /// @param heartbeatInterval heartbeat  전송 간격(밀리초 단위)
    ClientSideSessionConfig(msec_t heartbeatInterval,
        std::unique_ptr<base::MemoryBlockManager> memoryBlockManager,
        std::unique_ptr<sgp::PacketCoder> packetCoder,
        base::SessionDestroyer& sessionDestroyer,
        base::IoContextTask& ioContextTask,
        const base::SessionCapacity& capacity) :
        sgp::SgpSessionConfig(std::move(memoryBlockManager),
            std::move(packetCoder),
            sgp::PacketSeedExchangerFactory::createForClient(),
            sessionDestroyer, ioContextTask, capacity),
        heartbeatInterval_(heartbeatInterval) {}

    bool isValid() const {
        return sgp::SgpSessionConfig::isValid();
    }
};

}} // namespace sne { namespace server {
