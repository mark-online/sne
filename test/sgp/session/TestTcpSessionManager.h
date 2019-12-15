#pragma once

#include "TestTcpSession.h"
#include <sne/sgp/session/SgpSessionImplFactory.h>
#include <sne/sgp/session/SgpSessionConfig.h>
#include <sne/sgp/protocol/security/PacketSeedExchangerFactory.h>
#include <sne/sgp/protocol/PacketCoderFactory.h>
#include <sne/base/session/manager/SessionManager.h>
#include <sne/base/memory/MemoryBlockManager.h>

/**
 * @class TestTcpSessionManager
 *
 * 테스트용 SessionManager
 * - 메모리 누수가 발생하지만 의도적으로 놔둔 것임. 중요한 것은 세션 생성 여부!
 */
class TestTcpSessionManager : public base::SessionManager
{
    typedef std::set<base::Session*> SessionSet;
public:
    TestTcpSessionManager(base::IoContextTask& ioContextTask) :
        ioContextTask_(ioContextTask),
        lastSession_(nullptr) {}

    virtual ~TestTcpSessionManager() {
        std::lock_guard<std::mutex> lock(lock_);

        for (base::Session* session : sessionSet_) {
            delete session;
        }
    }

    virtual void initialize() override {}

    virtual base::Session* acquire() override {
        std::lock_guard<std::mutex> lock(lock_);

        std::unique_ptr<base::MemoryBlockManager> memoryBlockManager(
            std::make_unique<base::MemoryBlockManager>(10, 10));

        sgp::SgpSessionConfig config(std::move(memoryBlockManager),
            sgp::PacketCoderFactory().create(),
            sgp::PacketSeedExchangerFactory::createForServer(),
            *this, ioContextTask_, base::SessionCapacity::getUnlimitedCapacity());
        auto session = new TestTcpSession(
            sgp::SgpSessionImplFactory::create(config));

        sessionSet_.insert(session);
        return session;
    }

    virtual void accepted(base::Session& session) override {
        lastSession_ = static_cast<TestTcpSession*>(&session);
    }

    virtual void release(base::Session* session) override {
        std::lock_guard<std::mutex> lock(lock_);

        if (session == lastSession_) {
            lastSession_ = nullptr;
        }
        //session->disconnect();
        sessionSet_.erase(session);
        // 의도적인 memory leak
        //delete session;
    }

    virtual void cancel() override {
        SessionSet copiedSessionSet;
        {
            std::lock_guard<std::mutex> lock(lock_);
            copiedSessionSet = sessionSet_;
        }

        for (base::Session* session : copiedSessionSet) {
            session->disconnect();
        }
    }

    virtual void waitForDone() override {}

    virtual bool isSafeToDelete() const override {
        return true;
    }

    virtual size_t getSessionCount() const override {
        std::lock_guard<std::mutex> lock(lock_);

        return sessionSet_.size();
    }

    virtual size_t getTotalSessionCount() const override {
        std::lock_guard<std::mutex> lock(lock_);

        return 0;
    }

public:
    TestTcpSession* getLastSession() const {
        std::lock_guard<std::mutex> lock(lock_);

        return lastSession_;
    }

private:
    base::IoContextTask& ioContextTask_;
    TestTcpSession* lastSession_;
    SessionSet sessionSet_;

    mutable std::mutex lock_;
};
