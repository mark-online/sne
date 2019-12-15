#pragma once

#include "TestTcpSession.h"
#include <sne/base/session/manager/CachedSessionManager.h>
#include <sne/base/session/manager/SessionFactory.h>
#include <sne/sgp/protocol/security/PacketSeedExchangerFactory.h>


/**
 * @class TestTcpSessionFactory
 */
class TestTcpSessionFactory : public base::SessionFactory
{
public:
    TestTcpSessionFactory(base::IoContextTask& ioContextTask) :
        ioContextTask_(ioContextTask),
        sessionDestroyer_(nullptr) {}

    virtual void setSessionDestroyer(
        base::SessionDestroyer& sessionDestroyer) override {
        sessionDestroyer_ = &sessionDestroyer;
    }

    virtual std::unique_ptr<base::Session> create() const override {
        std::unique_ptr<base::MemoryBlockManager> memoryBlockManager(
            std::make_unique<base::MemoryBlockManager>(10, 10));
        std::unique_ptr<sgp::PacketCoder> packetCoder =
            sgp::PacketCoderFactory().create();
        sgp::SgpSessionConfig config(
            std::move(memoryBlockManager), std::move(packetCoder),
            sgp::PacketSeedExchangerFactory::createForServer(),
            *sessionDestroyer_, ioContextTask_,
            base::SessionCapacity::getUnlimitedCapacity());
        std::unique_ptr<base::SessionImpl> sessionImpl(
            sgp::SgpSessionImplFactory::create(config));
        return std::unique_ptr<base::Session>(
            std::make_unique<TestTcpSession>(std::move(sessionImpl)));
    }

private:
    base::IoContextTask& ioContextTask_;
    base::SessionDestroyer* sessionDestroyer_;
};


/**
 * @class TestCachedSessionManager
 *
 * 테스트용 CachedSessionManager
 */
class TestCachedSessionManager : public base::CachedSessionManager
{
public:
    enum {
        poolSize = 2
    };
public:
    TestCachedSessionManager(base::SessionFactory& sessionFactory) :
        base::CachedSessionManager("ForTest", poolSize, sessionFactory),
        lastSession_(nullptr) {}

    virtual base::Session* acquire() override {
        lastSession_ = base::CachedSessionManager::acquire();
        return lastSession_;
    }

private:
    base::Session* lastSession_;
};
