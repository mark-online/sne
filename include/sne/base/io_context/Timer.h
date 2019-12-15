#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace base {

/** @addtogroup io_service
* @{
*/

/**
 * @class TimerCallback
 * Timer callback
 */
class TimerCallback : public boost::noncopyable
{
public:
    virtual ~TimerCallback() {}

    virtual void timedOut(const void* act) = 0;
};


/**
 * @class Timer
 * Timer interface
 */
class Timer : public boost::noncopyable
{
public:
    virtual ~Timer() {}

    /// 일회성 타이머를 설정한다
    /// @pre ! isRunning()
    /// @param act TimerCallback에 전달되는 ACT.
    virtual bool once(msec_t timeout, const void* act = nullptr) = 0;

    /// 주기적으로 반복 호출되는 타이머를 설정한다
    /// @pre ! isRunning()
    /// @param act TimerCallback에 전달되는 ACT.
    virtual bool repeat(msec_t timeout, const void* act = nullptr) = 0;

    /// 타이머를 취소한다
    /// - 일회성 타이머의 경우 취소하는 것이 아니라면 명시적으로 호출하지 않아도 된다.
    virtual void cancel() = 0;

    /// 타이머가 설정 되었는가?
    /// - 일회성 타이머의 경우 시작 후 종료 직전인가?
    /// - 반복 타이머의 경우 시작되었는가?
    virtual bool isRunning() const = 0;
};

/** @} */ // addtogroup io_service

}} // namespace sne { namespace base {
