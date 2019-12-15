#include "SgpTestPCH.h"
#include "TcpSessionTestFixture.h"
#include "TestClient.h"
#include "TestCachedSessionManager.h"


const size_t  clientCount = 8;

/**
* @class CachedSessionManagerTest
*
* TestCachedSessionManager Test
*/
class CachedSessionManagerTest : public TcpSessionTestFixture
{
public:
    CachedSessionManagerTest() {
        for (size_t i = 0; i < clientCount; ++i) {
            client_[i] = nullptr;
        }
    }
private:
    virtual void SetUp() override {
        TcpSessionTestFixture::SetUp();

        ASSERT_EQ(getAsioAcceptorCount(),
            getSessionManager()->getActiveSessionCount());

        connect();
    }

    virtual void TearDown() override {
        disconnect();

        TcpSessionTestFixture::TearDown();

        delete sessionFactory_;
    }

    virtual base::SessionManager* createSessionManager() override {
        sessionFactory_ = new TestTcpSessionFactory(*ioContextTask_);
        return new TestCachedSessionManager(*sessionFactory_);
    }

protected:
    void connect();
    void disconnect();

    TestCachedSessionManager* getSessionManager() {
        return static_cast<TestCachedSessionManager*>(sessionManager_);
    }

protected:
    TestTcpSessionFactory* sessionFactory_;
    TestClient* client_[clientCount];
};


void CachedSessionManagerTest::connect()
{
    for (size_t i = 0; i < clientCount; ++i) {
        client_[i] = new TestClient;
        ASSERT_TRUE(client_[i]->connect(getTestAddress().address_, getTestAddress().port_)) <<
            "#" << i << " connect";
    }

    base::pause(10);
}


void CachedSessionManagerTest::disconnect()
{
    for (size_t i = 0; i < clientCount; ++i) {
        if (client_[i] != nullptr) {
            client_[i]->close();
            delete client_[i];
            client_[i] = nullptr;
        }
    }
}


TEST_F(CachedSessionManagerTest, testAcquire)
{
    ASSERT_EQ(clientCount + getAsioAcceptorCount(),
        getSessionManager()->getActiveSessionCount());
    ASSERT_GE(getSessionManager()->getInactiveSessionCount(), 0);
}



TEST_F(CachedSessionManagerTest, testRelease)
{
    disconnect();

    base::pause(10);

    ASSERT_EQ(getAsioAcceptorCount(),
        getSessionManager()->getActiveSessionCount());
    ASSERT_GE(getSessionManager()->getInactiveSessionCount(), clientCount);
}


TEST_F(CachedSessionManagerTest, testCancel)
{
    sessionManager_->cancel();

    base::pause(10);

    ASSERT_EQ(getAsioAcceptorCount(),
        getSessionManager()->getActiveSessionCount());
    ASSERT_GE(getSessionManager()->getInactiveSessionCount(), clientCount);
}


TEST_F(CachedSessionManagerTest, testReuseSession)
{
    disconnect();
    connect();
    base::pause(10);

    ASSERT_EQ(clientCount + getAsioAcceptorCount(),
        getSessionManager()->getActiveSessionCount());
    ASSERT_GE(getSessionManager()->getInactiveSessionCount(), 0);
}
