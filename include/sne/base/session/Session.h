#pragma once

#include "detail/SessionCallback.h"
#include "detail/SessionStats.h"
#include "impl/SessionImpl.h"
#include "extension/SessionExtension.h"
#include <sne/Common.h>

namespace sne { namespace base {

class SessionImpl;
class IoContext;
struct Message;

/** @addtogroup session
* @{
*/

/**
 * @class Session
 *
 * 네트웍 연결 세션
 * - public API 주석은 SessionImpl 클래스 참고
 * - Bridge pattern
 */
class SNE_API Session :
    protected SessionCallback
{
public:
    Session();
    virtual ~Session();

    void initialize();
    void finalize();

    void setImplementation(std::unique_ptr<SessionImpl> impl);

    bool connect(const std::string& host, uint16_t port, msec_t timeout);

    /// 세션 연결을 취소(해제)한다
    void disconnect();

    /// 전송을 완료한 후 접속을 해제한다
    void disconnectGracefully();

    /// 버퍼를 전송한다
    void sendMessage(const void* buffer, size_t bufferSize,
        MessageType messageType);

public:
    bool isConnected() const;

    /// 세션 통계 정보를 얻는다
    SessionStats getStats() const;

public:
    IoContext& getIoContext();

    SessionImpl& getImpl() {
        assert(hasImpl());
        return *impl_;
    }

    bool hasImpl() const {
        return impl_.get() != nullptr;
    }

public:
    template <typename Extension>
    Extension* getExtension() {
        return static_cast<Extension*>(getExtension(Extension::getId()));
    }

    SessionExtension* getExtension(SessionExtension::Id extId);

protected:
    // = SessionCallback overriding
    virtual bool onMessageArrived(MessageType messageType,
        MemoryBlock& mblock) override;
    virtual void onRelease() override;

private:
    std::unique_ptr<SessionImpl> impl_;
};

}} // namespace sne { namespace base {
