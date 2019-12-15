#pragma once

#include "config/ServerSideSessionConfig.h"
#include "../common/ServerSpec.h"
#include <sne/base/session/manager/SessionFactory.h>
#include <sne/Common.h>

namespace sne { namespace sgp {
class SecurePacketCoderFactory;
}} // namespace sne { namespace sgp {

namespace sne { namespace server {

/**
 * @class AbstractServerSideSessionFactory
 *
 * Server-side Session Factory
 */
class SNE_API AbstractServerSideSessionFactory :
    public base::SessionFactory
{
public:
    AbstractServerSideSessionFactory(const ServerSpec& serverSpec,
        base::IoContextTask& ioContextTask);
    virtual ~AbstractServerSideSessionFactory();

private:
    virtual void setSessionDestroyer(
        base::SessionDestroyer& sessionDestroyer) override {
        sessionConfig_.sessionDestroyer_ = &sessionDestroyer;
    }

protected:
    ServerSideSessionConfig makeSessionConfig() const;

    const ServerSpec& getServerSpec() const {
        return serverSpec_;
    }

private:
    std::unique_ptr<sgp::PacketCoder> createPacketCoder() const;

private:
    ServerSpec serverSpec_;
    ServerSideSessionConfig sessionConfig_;
    std::unique_ptr<sgp::SecurePacketCoderFactory> packetCoderFactory_;
    std::unique_ptr<base::MemoryBlockManager> memoryBlockManager_;
};

}} // namespace sne { namespace server {
