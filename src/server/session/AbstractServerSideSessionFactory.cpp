#include "ServerPCH.h"
#include <sne/server/session/AbstractServerSideSessionFactory.h>
#include <sne/sgp/protocol/security/SecurePacketCoderFactory.h>
#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/base/memory/MemoryBlockManager.h>
#include <sne/base/utility/Assert.h>

namespace sne { namespace server {

AbstractServerSideSessionFactory::AbstractServerSideSessionFactory(
    const ServerSpec& serverSpec, base::IoContextTask& ioContextTask) :
    serverSpec_(serverSpec),
    packetCoderFactory_(
        std::make_unique<sgp::SecurePacketCoderFactory>(
          serverSpec.packetCipher_, serverSpec.cipherKeyTimeLimit_))
{
    assert(serverSpec_.isValid());

    sessionConfig_.ioContextTask_ = &ioContextTask;
    sessionConfig_.sessionCapacity_ = serverSpec_.sessionCapacity_;

    auto dummyPacketCoder = createPacketCoder();
    memoryBlockManager_ = std::make_unique<base::MemoryBlockManager>(
        dummyPacketCoder->getDefaultPacketPoolSize(), dummyPacketCoder->getDefaultPacketSize());

}


AbstractServerSideSessionFactory::~AbstractServerSideSessionFactory()
{
}


ServerSideSessionConfig AbstractServerSideSessionFactory::makeSessionConfig() const
{
    ServerSideSessionConfig sessionConfig = sessionConfig_;
    sessionConfig.packetSeedExchanger_ =
        sgp::PacketSeedExchangerFactory::createForServer().release();
    sessionConfig.packetCoder_ = createPacketCoder().release();
    sessionConfig.memoryBlockManager_ = memoryBlockManager_.get();

    SNE_ASSERT(sessionConfig.isValid());
    return sessionConfig;
}


std::unique_ptr<sgp::PacketCoder>
AbstractServerSideSessionFactory::createPacketCoder() const
{
    return packetCoderFactory_->create();
}

}} // namespace sne { namespace server {
