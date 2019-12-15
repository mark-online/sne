#pragma once

namespace sne { namespace base {

/** @addtogroup session
* @{
*/

/**
 * @struct SessionCapacity
 * - 만일 초당 maxBytesPerSecond_ 바이트 이상 수신된다면,
 *   secondsForThrottling_ 초만큼 대기한 후 처리를 한다.
 */
struct SessionCapacity
{
    /// 초당 수신 가능한 바이트 수. 0이면 제한 없음
    uint16_t maxBytesPerSecond_;
    /// 수신량이 제한치를 넘어설 경우 대기할 시간 간격(초 단위)
    uint16_t secondsForThrottling_;
    /// 송신 큐에 넣을 수 있는 총 패킷 갯수. 초과할 경우 접속을 해제한다.
    /// 0이면 제한 없음
    uint16_t maxPendablePacketCount_;

    SessionCapacity(uint16_t maxBytesPerSecond,
        uint16_t secondsForThrottling, uint16_t maxPendablePacketCount) :
        maxBytesPerSecond_(maxBytesPerSecond),
        secondsForThrottling_(secondsForThrottling),
        maxPendablePacketCount_(maxPendablePacketCount) {}

    bool shouldThrottle() const {
        return maxBytesPerSecond_ > 0;
    }

    static SessionCapacity getUnlimitedCapacity() {
        return SessionCapacity(0, 0, 0);
    }
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
