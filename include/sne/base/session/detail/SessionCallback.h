#pragma once

#include "../../protocol/Message.h"
#include <boost/noncopyable.hpp>

namespace sne { namespace base {

class MemoryBlock;

/** @addtogroup session
* @{
*/

/**
 * @class SessionCallback
 */
class SessionCallback : public boost::noncopyable
{
public:
    virtual ~SessionCallback() {}

    /// 접속될 경우 호출된다.
    /// @return false이면 접속을 해제한다
    virtual bool onConnected() = 0;

    /// 접속이 해제될 경우 호출된다.
    virtual void onDisconnected() = 0;

    /// 메시지를 전송하였다
    virtual void onMessageTransfered(MessageType messageType) = 0;

    /// 메시지가 도착하였다.
    /// @return 메시지를 처리하면 true, 아니면 false 리턴
    virtual bool onMessageArrived(MessageType messageType,
        MemoryBlock& mblock) = 0;

    /// throttling이 발생하였다
    virtual void onThrottling(size_t readBytes, size_t maxBytesPerSecond) = 0;

    /// 세션을 반납한다
    virtual void onRelease() = 0;
};

}} // namespace sne { namespace base {
