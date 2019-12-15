#pragma once

#include "SessionCapacity.h"

namespace sne { namespace base {

class IoContextTask;
class SessionDestroyer;

/** @addtogroup session
* @{
*/

/**
* @struct SessionConfig
*/
struct SessionConfig
{
    /// 유니코드 문자열(WString) 전송시 UTF-8로 변환할 것인가?
    bool shouldUseUtf8ForString_;

    /// 비트 압축을 사용할 것인가?
    bool useBitPacking_;

    SessionDestroyer* sessionDestroyer_;
    IoContextTask* ioContextTask_;

    SessionCapacity sessionCapacity_;

    bool isForTest_;

    SessionConfig() :
        shouldUseUtf8ForString_(false),
        useBitPacking_(false),
        sessionDestroyer_(nullptr),
        ioContextTask_(nullptr),
        sessionCapacity_(SessionCapacity::getUnlimitedCapacity()),
        isForTest_(false) {}

    /**
    * ctor
    * @param sessionDestroyer 세션 관리자 인스턴스.
    * @param ioService_ IoService 인스턴스.
    */
    SessionConfig(SessionDestroyer& sessionDestroyer,
        IoContextTask& ioContextTask,
        const SessionCapacity& capacity) :
        shouldUseUtf8ForString_(false),
        useBitPacking_(false),
        sessionDestroyer_(&sessionDestroyer),
        ioContextTask_(&ioContextTask),
        sessionCapacity_(capacity) {}

    bool isValid() const {
        return (sessionDestroyer_ != nullptr) && (ioContextTask_ != nullptr);
    }
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
