#pragma once

#include "TestTcpSessionManager.h"
#include <sne/sgp/protocol/PacketCoderFactory.h>
#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/base/io_context/IoContextAcceptor.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/io_context/IoContext.h>
#include <sne/base/utility/SystemUtil.h>
#include <gtest/gtest.h>

using namespace sne;

/**
* @class TcpSessionTestFixture
*
* BOOST ASIO based Session TestFixture
*/
class TcpSessionTestFixture : public testing::Test
{
    enum { defaultServerPort = 34567 };

protected:
    virtual void SetUp() override {
        ioContextTask_ = new base::IoContextTask("io_context");
        ioContextTask_->start(1);

        sessionManager_ = createSessionManager();
        sessionManager_->initialize();

        acceptor_ = new base::IoContextAcceptor(*ioContextTask_,
            *sessionManager_, *sessionManager_);
        (void)acceptor_->open(getTestAddress());

        packetCoder_ = sgp::PacketCoderFactory().create().release();
    }

    virtual void TearDown() override {
        if (acceptor_ != nullptr) {
            acceptor_->close();
            //base::pause(1);
        }

        if (sessionManager_ != nullptr) {
            sessionManager_->cancel();
            sessionManager_->waitForDone();
        }

        if (ioContextTask_ != nullptr) {
            ioContextTask_->stop();
            ioContextTask_->waitForDone();
        }

        delete acceptor_;
        delete sessionManager_;
        delete ioContextTask_;

        delete packetCoder_;
    }

private:
    virtual base::SessionManager* createSessionManager() {
        return new TestTcpSessionManager(*ioContextTask_);
    }

protected:
    base::InetAddress getTestAddress() const {
        return base::InetAddress(defaultServerPort, "localhost");
    }

    size_t getAsioAcceptorCount() const {
        return acceptor_->getAsioAcceptorCount();
    }

protected:
    base::IoContextTask* ioContextTask_ = nullptr;
    base::IoContextAcceptor* acceptor_ = nullptr;

    base::SessionManager* sessionManager_ = nullptr;
    sgp::PacketCoder* packetCoder_ = nullptr;
};
