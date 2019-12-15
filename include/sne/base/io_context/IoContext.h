#pragma once

#include <sne/Common.h>
#include <boost/asio/io_context.hpp>

namespace sne { namespace base {

class Timer;
class TimerCallback;

/** @addtogroup io_context
* @{
*/

/**
 * @class IoContext
 * BOOST ASIO io_context를 래핑한다
 */
class SNE_API IoContext : public boost::asio::io_context
{
public:
    /// 타이머 객체를 생성한다
    std::unique_ptr<Timer> createTimer(TimerCallback& callback);
};

/** @} */ // addtogroup io_context

}} // namespace sne { namespace base {
