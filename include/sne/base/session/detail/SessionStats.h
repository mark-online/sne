#pragma once

namespace sne { namespace base {

/** @addtogroup session
* @{
*/

/**
 * @struct SessionStats
 * 세션의 통계 정보를 기록한다.
 */
struct SessionStats
{
    size_t recvBytes_;
    size_t sentBytes_;
    size_t recvMessageCount_;
    size_t sentMessageCount_;
    size_t useCount_;

    SessionStats() :
        useCount_(0) {
        reset();
    }

    void reset() {
        recvBytes_ = 0;
        sentBytes_ = 0;
        recvMessageCount_ = 0;
        sentMessageCount_ = 0;
        ////useCount_ = 0;
    }
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
