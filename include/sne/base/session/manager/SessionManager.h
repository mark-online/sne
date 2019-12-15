#pragma once

#include "SessionCreator.h"
#include "SessionDestroyer.h"
#include <boost/noncopyable.hpp>

namespace sne { namespace base {

/** @addtogroup session
* @{
*/


/**
 * @class SessionManagerCallback
 */
class SessionManagerCallback : public boost::noncopyable
{
public:
    virtual ~SessionManagerCallback() {}

    virtual void sessionAcquired(size_t acquiredSessionCount) = 0;

    virtual void sessionReleased(size_t acquiredSessionCount) = 0;
};


/**
 * @class SessionManager
 *
 * Asynchronous Session Manager
 */
class SessionManager :
    public SessionCreator, public SessionDestroyer, public boost::noncopyable
{
public:
    virtual ~SessionManager() {}

    /// SessionManager를 초기화한다. 제2의 ctor.
    virtual void initialize() = 0;

    /// 현재 사용 중인 모든 세션의 AIO를 취소하고 세션 생성(acquire)을 못하게
    /// 막는다.
    virtual void cancel() = 0;

    /// 안전하게 삭제할 수 있을 때까지 대기합니다.
    virtual void waitForDone() = 0;

    /// 안전하게 메모리를 해제할 수 있는가?
    virtual bool isSafeToDelete() const = 0;

    /// 세션수를 얻는다
    virtual size_t getSessionCount() const = 0;

    /// 전체 세션수(풀)를 얻는다
    virtual size_t getTotalSessionCount() const = 0;

};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
