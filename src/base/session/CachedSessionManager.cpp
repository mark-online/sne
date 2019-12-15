#include "BasePCH.h"

#include <sne/base/session/Session.h>
#include <sne/base/session/manager/CachedSessionManager.h>
#include <sne/base/session/manager/SessionFactory.h>
#include <sne/base/session/manager/SessionPool.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace base {

/**
* @class SessionAllocator
*
* CachedSessionManager와 SessionPool을 연결하기 위한 helper class.
*/
class SessionAllocator
{
public:
    SessionAllocator(SessionFactory& sessionFactory,
        SessionDestroyer& sessionDestroyer) :
        sessionFactory_(sessionFactory) {
        sessionFactory_.setSessionDestroyer(sessionDestroyer);
    }

    Session* malloc() {
        return sessionFactory_.create().release();
    }

    void free(Session* session) {
        assert(session != nullptr);
        boost::checked_delete(session);
    }
private:
    SessionFactory& sessionFactory_;
};

// = CachedSessionManager

// poolSize의 10%를 여분으로 둠
CachedSessionManager::CachedSessionManager(const std::string& name,
    size_t poolSize, SessionFactory& sessionFactory,
    SessionManagerCallback* callback) :
    sessionAllocator_(
        std::make_unique<SessionAllocator>(sessionFactory, *this)),
    sessionPool_(
        new CachedSessionPool(poolSize, *sessionAllocator_, this)),
    name_(name),
    callback_(callback)
{
}


CachedSessionManager::~CachedSessionManager()
{
}


void CachedSessionManager::initialize()
{
    sessionPool_->initialize();
}


Session* CachedSessionManager::acquire()
{
    Session* session = sessionPool_->acquire();
    if (! session) {
        return nullptr;
    }

    const size_t sessionCount = sessionPool_->getActiveResourceCount();
    SNE_LOG_DEBUG("CachedSessionManager(%s) - %d session(s) acquired.",
        name_.c_str(), sessionCount);

    if (callback_ != nullptr) {
        callback_->sessionAcquired(sessionCount);
    }
    return session;
}


void CachedSessionManager::release(Session* session)
{
    assert(session != nullptr);

    sessionPool_->release(session);

    const size_t sessionCount = sessionPool_->getActiveResourceCount();
    SNE_LOG_DEBUG("CachedSessionManager(%s) - %d session(s) left.",
        name_.c_str(), sessionCount);

    if (callback_ != nullptr) {
        callback_->sessionReleased(sessionCount);
    }
}


void CachedSessionManager::cancel()
{
    sessionPool_->cancel();
}


void CachedSessionManager::waitForDone()
{
    while (! isSafeToDelete()) {
        pause(1);
    }
}


size_t CachedSessionManager::getActiveSessionCount() const
{
    return sessionPool_->getActiveResourceCount();
}


size_t CachedSessionManager::getInactiveSessionCount() const
{
    return sessionPool_->getInactiveResourceCount();
}

// = MemoryPoolCallback overriding

void CachedSessionManager::poolGrowing(size_t neededSize)
{
    SNE_LOG_DEBUG("CachedSessionManager(%s) - SessionPool growing by %u.",
        name_.c_str(), neededSize);
}


void CachedSessionManager::poolGrowed(size_t activeResourceSize,
    size_t inactiveResourceSize)
{
    SNE_LOG_DEBUG("CachedSessionManager(%s) - SessionPool growed(%u,%u).",
        name_.c_str(), activeResourceSize, inactiveResourceSize);
}

}} // namespace sne { namespace base {
