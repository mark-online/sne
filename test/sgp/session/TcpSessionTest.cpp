#include "SgpTestPCH.h"
#include "TcpSessionTestFixture.h"
#include "TestClient.h"


/**
* @class TcpSessionTest
*
* Session Test
*/
class TcpSessionTest : public TcpSessionTestFixture
{
private:
    virtual void SetUp() override {
        TcpSessionTestFixture::SetUp();

        client_ = new TestClient;
        (void)client_->connect(getTestAddress().address_, getTestAddress().port_);

        base::pause(10);
        ASSERT_TRUE(getLastSession() != nullptr);
    }

    virtual void TearDown() override {
        TcpSessionTestFixture::TearDown();

        delete client_;
    }

protected:
    TestTcpSession* getLastSession() {
        return static_cast<TestTcpSessionManager*>(sessionManager_)->getLastSession();
    }

protected:
    TestClient* client_ = nullptr;
};


TEST_F(TcpSessionTest, testRecvPackets)
{
    const int sendCount = 5;
    const uint8_t body[] = "1234567890";
    const uint16_t bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        client_->sendMessage(body, bodySize);
    }

    base::pause(1);

    ASSERT_EQ((packetCoder_->getHeaderSize() + bodySize) * sendCount,
        getLastSession()->getStats().recvBytes_);
    ASSERT_EQ(sendCount, int(getLastSession()->getArrivedMessageCount()));
}


TEST_F(TcpSessionTest, testSendPackets)
{
    const int sendCount = 5;
    const uint8_t body[] = "1234567890";
    const uint16_t bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    TestTcpSession* session = getLastSession();

    for (int i = 0; i < sendCount; ++i) {
        session->sendMessage(body, bodySize, base::mtFirst);
    }

    session->disconnectGracefully();

    base::pause(10);

    for (int i = 0; i < sendCount; ++i) {
        uint8_t received[bodySize];
        ASSERT_EQ(bodySize, client_->recvMessage(received, bodySize)) <<
            "received size, #" << i;
        ASSERT_TRUE(memcmp(body, received, bodySize) == 0);
    }

    ASSERT_EQ((packetCoder_->getHeaderSize() + bodySize) * sendCount,
        session->getStats().sentBytes_) << "sent bytes";
}


TEST_F(TcpSessionTest, testConnect)
{
    TestTcpSession* session =
        static_cast<TestTcpSession*>(sessionManager_->acquire());

    ASSERT_TRUE(
        session->connect(getTestAddress().address_, getTestAddress().port_, 10));

    base::pause(1);

    const char body[] = "1234567890";
    const uint16_t bodySize = (sizeof(body) / sizeof(body[0])) - 1;
    getLastSession()->sendMessage(body, bodySize, base::mtFirst);

    base::pause(10);

    ASSERT_EQ(1, int(session->getArrivedMessageCount())) <<
        "received count";
}
