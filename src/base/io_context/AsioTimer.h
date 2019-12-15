#pragma once

#include <sne/base/io_context/Timer.h>
#include <sne/base/utility/Logger.h>
#include <boost/asio/deadline_timer.hpp>

namespace sne { namespace base {

/** @addtogroup io_context
* @{
*/

/**
 * @class AsioTimer
 * BOOST ASIO 기반의 Timer
 */
class AsioTimer : public Timer
{
public:
    AsioTimer(boost::asio::io_context& ioContext, TimerCallback& callback) :
        timer_(ioContext),
        callback_(callback),
        isRunning_(false) {}

    virtual ~AsioTimer() {
        if (isRunning_) {
            boost::system::error_code error;
            timer_.cancel(error);
        }
    }

private:
    virtual bool once(msec_t timeout, const void* act) override {
        assert(! isRunning_);

        timer_.expires_from_now(boost::posix_time::milliseconds(timeout));
        timer_.async_wait(
            boost::bind(&AsioTimer::handle_timeout, this, _1, act));
        isRunning_ = true;
        return true;
    }

    virtual bool repeat(msec_t timeout, const void* act) override {
        //assert(! isRunning_);

        timer_.expires_from_now(boost::posix_time::milliseconds(timeout));
        timer_.async_wait(
            boost::bind(&AsioTimer::handle_repeat_timeout, this, _1, timeout, act));
        isRunning_ = true;
        return true;
    }

    virtual void cancel() override {
        if (isRunning_) {
            boost::system::error_code error;
            timer_.cancel(error);
            if (error) {
                SNE_LOG_DEBUG("AsioTimer error = %s", error.message().c_str());
            }
        }
        isRunning_ = false;
    }

    virtual bool isRunning() const override {
        return isRunning_;
    }

private:
    void handle_timeout(const boost::system::error_code& ec, const void* act) {
        if (!ec) {
            if (isRunning_) {
                isRunning_ = false;
                callback_.timedOut(act);
            }
        }
        else {
            if (ec != boost::asio::error::operation_aborted) {
                SNE_LOG_DEBUG("AsioTimer error = %s", ec.message().c_str());
            }
        }
        isRunning_ = false;
    }

    void handle_repeat_timeout(const boost::system::error_code& ec,
        msec_t timeout, const void* act) {
        if (!ec) {
            if (isRunning_) {
                repeat(timeout, act);
                callback_.timedOut(act);
            }
        }
        else {
            isRunning_ = false;
            if (ec != boost::asio::error::operation_aborted) {
                SNE_LOG_DEBUG("AsioTimer error = %s", ec.message().c_str());
            }
        }
    }

private:
    boost::asio::deadline_timer timer_;
    TimerCallback& callback_;
    volatile bool isRunning_;
};

/** @} */ // addtogroup io_context

}} // namespace sne { namespace base {
