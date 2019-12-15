#pragma once

#include "SessionManager.h"
#include "../../memory/MemoryPool.h"
#include <sne/Common.h>

namespace sne { namespace base {

class SessionAllocator;
class SessionFactory;
class SessionManagerCallback;
template <class Allocator, class Mutex> class SessionPool;


/** @addtogroup session
* @{
*/

/**
 * @class CachedSessionManager
 *
 * Cached(Pooling) Session Manager
 * - Session 인스턴스는 재사용이 가능하므로 새로운 세션이 시작될 경우
 *   Session::open()을 오버라이딩하여 적절하게 초기화를 해주어야 한다.
 *
 */
class SNE_API CachedSessionManager : public SessionManager,
    private MemoryPoolCallback
{
    typedef SessionPool<SessionAllocator, std::mutex> CachedSessionPool;

public:
    /**
     * ctor
     * @param name 고유 이름. 디버깅 용도로 사용.
     * @param poolSize Session Pool 크기. 가득 찰 경우 poolSize만큼 세션을 생성
     *                 한다.
     * @param sessionFactory 새로운 세션을 만들 팩토리 인스턴스.
     */
    CachedSessionManager(const std::string& name, size_t poolSize,
        SessionFactory& sessionFactory, SessionManagerCallback* callback = nullptr);

    virtual ~CachedSessionManager();

    /// 사용 중인 세션 수를 얻는다.
    size_t getActiveSessionCount() const;

    /// 대기 중인 세션 수를 얻는다.
    size_t getInactiveSessionCount() const;

public: // for Test
    // = implementation
    virtual void initialize() override;
    virtual Session* acquire() override;
    virtual void release(Session* session) override;
    virtual void cancel() override;
    virtual void waitForDone() override;
    virtual bool isSafeToDelete() const override {
        return getActiveSessionCount() == 0;
    }
    virtual size_t getSessionCount() const override {
        return getActiveSessionCount();
    }

    virtual size_t getTotalSessionCount() const override {
        return getActiveSessionCount() + getInactiveSessionCount();
    }

private:
    // = MemoryPoolCallback overriding
    virtual void poolGrowing(size_t neededSize) override;
    virtual void poolGrowed(size_t activeResourceSize,
        size_t inactiveResourceSize) override;

private:
    /// 이 세션 매니저에서 공유할 메시지 블럭 매니저.
    std::unique_ptr<SessionAllocator> sessionAllocator_;
    std::unique_ptr<CachedSessionPool> sessionPool_;
    std::string name_;
    SessionManagerCallback* callback_;
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
