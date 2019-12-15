#include "SgpTestPCH.h"
#include "TcpSessionTestFixture.h"
#include "TestClient.h"


/**
* @class TcpSessionAcceptorTest
*
* IoSessionAcceptor Test
*/
class TcpSessionAcceptorTest : public TcpSessionTestFixture
{
private:
    virtual void SetUp() override {
        TcpSessionTestFixture::SetUp();

        client_ = new TestClient;
        (void)client_->connect(getTestAddress().address_, getTestAddress().port_);
    }

    virtual void TearDown() override {
        if (client_ != nullptr) {
            client_->close();
            delete client_;
        }

        TcpSessionTestFixture::TearDown();
    }

protected:
    TestTcpSessionManager* getSessionManager() {
        return static_cast<TestTcpSessionManager*>(sessionManager_);
    }

protected:
    TestClient* client_ = nullptr;
};


TEST_F(TcpSessionAcceptorTest, testAccept)
{
    base::pause(1);

    ASSERT_EQ(getAsioAcceptorCount() + 1,
        getSessionManager()->getSessionCount());
}


TEST_F(TcpSessionAcceptorTest, testMultipleAccept)
{
    const size_t connectionCount = 5;
    TestClient client[connectionCount];
    for (size_t i = 0; i < connectionCount; ++i) {
        ASSERT_TRUE(client[i].connect(getTestAddress().address_, getTestAddress().port_));
    }

    base::pause(100);

    ASSERT_EQ(getAsioAcceptorCount() + 1 + connectionCount,
        getSessionManager()->getSessionCount());
}


TEST_F(TcpSessionAcceptorTest, testStopToAccept)
{
    acceptor_->close();

    TestClient client;
    ASSERT_FALSE(client.connect(getTestAddress().address_, getTestAddress().port_)) <<
        "can't connect";

    ASSERT_FALSE(acceptor_->open(getTestAddress())) << "can't restart";
}


TEST_F(TcpSessionAcceptorTest, testDisconnected)
{
    base::pause(1);

    ASSERT_TRUE(getSessionManager()->getLastSession()->isConnected());

    client_->close();

    base::pause(1);

    ASSERT_EQ(getAsioAcceptorCount(),
        getSessionManager()->getSessionCount());
}
