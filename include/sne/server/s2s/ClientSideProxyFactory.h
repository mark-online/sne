#pragma once

#include "ServerInfo.h"
#include "../session/config/ClientSideSessionConfig.h"
#include "../common/Property.h"
#include <sne/sgp/protocol/security/SecurePacketCoderFactory.h>
#include <sne/base/session/manager/SessionDestroyer.h>
#include <sne/base/memory/MemoryBlockManager.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace server {

/**
 * @struct ClientSideProxyFactory
 */
template <typename Proxy>
struct ClientSideProxyFactory : public boost::noncopyable
{
    static std::unique_ptr<Proxy> create(const ServerInfo& serverInfo,
        base::IoContextTask& ioContextTask) {
        std::unique_ptr<sgp::PacketCoder> packetCoder =
            sgp::SecurePacketCoderFactory(serverInfo.packetCipher_).create();

        std::unique_ptr<base::MemoryBlockManager> memoryBlockManager =
            createMemoryBlockManager(*packetCoder);

        std::unique_ptr<Proxy> proxy =
            std::make_unique<Proxy>(
                serverInfo,
                ClientSideSessionConfig(
                    serverInfo.heartbeatInterval_,
                    std::move(memoryBlockManager),
                    std::move(packetCoder),
                    NullSessionDestroyer::instance(),
                    ioContextTask,
                    base::SessionCapacity::getUnlimitedCapacity()));

        proxy->setConnectionTimeout(
            SNE_PROPERTIES::getProperty<msec_t>("s2s.connection_timeout"));
        return proxy;
    }

private:
    /**
    * @class NullSessionDestroyer
    */
    class NullSessionDestroyer : public base::SessionDestroyer
    {
    public:
        static NullSessionDestroyer& instance() {
            static NullSessionDestroyer destroyer;
            return destroyer;
        }
    private:
        virtual void release(base::Session* /*session*/) override {}
    };

private:
    static std::unique_ptr<base::MemoryBlockManager> createMemoryBlockManager(
        sgp::PacketCoder& packetCoder) {
        return std::make_unique<base::MemoryBlockManager>(
            packetCoder.getDefaultPacketPoolSize(),
            packetCoder.getDefaultPacketSize());
    }
};

}} // namespace sne { namespace server {
