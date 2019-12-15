#pragma once

#include <sne/base/session/Session.h>
#include <sne/base/utility/Logger.h>
#include <boost/core/ignore_unused.hpp>

using namespace sne;

/**
 * @class TestTcpSession
 *
 * 테스트용 TCP/IP Session
 */
class TestTcpSession : public base::Session
{
public:
    TestTcpSession(std::unique_ptr<base::SessionImpl> impl) :
        arrivedMessageCount_(0) {
        base::Session::setImplementation(std::move(impl));
    }

    size_t getArrivedMessageCount() const {
        return arrivedMessageCount_;
    }

private:
    // = SessionCallback overriding

    virtual bool onConnected() override {
        return true;
    }

    virtual void onMessageTransfered(base::MessageType messageType) override {
        boost::ignore_unused(messageType);
    }

    virtual bool onMessageArrived(base::MessageType messageType,
        base::MemoryBlock& mblock) override {
        boost::ignore_unused(messageType);
        boost::ignore_unused(mblock);
        ++arrivedMessageCount_;
        return true;
    }

    virtual void onThrottling(size_t readBytes,
        size_t maxBytesPerSecond) override {
        boost::ignore_unused(readBytes);
        boost::ignore_unused(maxBytesPerSecond);
    }

    /// 접속이 해제될 경우 호출된다.
    virtual void onDisconnected() override {}

private:
    size_t arrivedMessageCount_;
};
