#pragma once

#include "../detail/MemoryBlockProvider.h"
#include "../detail/SessionStats.h"
#include "../detail/SessionCallback.h"
#include "../extension/SessionExtension.h"
#include "../../utility/SystemUtil.h"
#include <sne/core/container/Containers.h>
#include <sne/Common.h>
#include <boost/asio.hpp>
#include <memory>

namespace sne { namespace core {
class IStream;
class OStream;
class Streamable;
class StreamFactory;
}} // namespace sne { namespace core {

namespace sne { namespace base {

struct SessionConfig;
class IoContext;
class SessionCallback;
class SessionDestroyer;
class Marshaller;
class MemoryBlockStreamBuffer;

/** @addtogroup session
* @{
*/

/**
 * @class SessionImpl
 *
 * 네트웍 연결 세션 Implemetation
 * - Bridge pattern
 * - extensionMap_은 Thread-UNSAFE 하므로 객체 초기화 이후에
 *   확장을 등록해선 안된다!!
 */
class SNE_API SessionImpl :
    public MemoryBlockProvider
{
    typedef core::HashMap<SessionExtension::Id, std::unique_ptr<SessionExtension>>
        ExtensionMap;

public:
    SessionImpl(const SessionConfig& config,
        std::unique_ptr<Marshaller> marshaller,
        core::StreamFactory& streamFactory);
    virtual ~SessionImpl();

    void setCallback(SessionCallback& callback) {
        callback_ = &callback;
    }

public:
    template <typename Extension>
    Extension* registerExtension() {
        const SessionExtension::Id extId = Extension::getId();
        assert(extensionMap_.find(extId) == extensionMap_.end());
        auto extension(std::make_unique<Extension>());
        Extension* savedExtension = extension.get();
        extensionMap_.emplace(extId, std::move(extension));
        savedExtension->attachedTo(*this);
        return savedExtension;
    }

    template <typename Extension>
    Extension* getExtension() {
        return static_cast<Extension*>(getExtension(Extension::getId()));
    }

    SessionExtension* getExtension(SessionExtension::Id extId);

    /// 수신된 메시지의 처리를 Extension에 의뢰한다
    /// @return 메시지를 처리했으면 true
    bool handleMessage(MessageType messageType,
        MemoryBlock& mblock);

public:
    /// OStream에 serializing한다. 실패할 경우 연결을 끊는다
    MemoryBlock* serialize(const void* buffer, size_t bufferSize);

    /// OStream에 serializing한다. 실패할 경우 연결을 끊는다
    MemoryBlock* serialize(core::Streamable& to);

    /// deserializing을 준비한다. 실패할 경우 연결을 끊는다
    core::IStream& deserialize(MemoryBlock& mblock);

public:
    SessionDestroyer& getSessionDestroyer() {
        return sessionDestroyer_;
    }

public:
    /// session이 초기화되었다
    virtual void open() = 0;

    /// remote host에 접속한다
    virtual bool connect(const std::string& host, uint16_t port, msec_t timeout) = 0;

    /// 세션 연결을 취소(해제)한다
    virtual void disconnect() = 0;

    /// 전송을 완료한 후 접속을 해제한다
    virtual void disconnectGracefully() = 0;

    /**
     * 메시지를 전송한다. 전송이 실패할 경우 접속을 해제한다.
     * @param mblock 메시지 블럭.
     * @param messageType 메시지 타입.
     *        MemoryBlockProvider::acquireSendBlock() 통해 획득해야 한다.
     */
    virtual void sendMessage(MemoryBlock& mblock, MessageType messageType) = 0;

public:
    /// 접속 중인가?
    virtual bool isConnected() const = 0;

    /// 통계 정보를 얻는다.
    virtual SessionStats getStats() const {
        return stats_;
    }

    virtual IoContext& getIoContext() = 0;

    /// 클라이언트 세션이면 서버의 IP 주소
    virtual InetAddress getRemoteAddress() const = 0;

    /// 클라이언트 세션이면 서버 측에 바인딩된 클라이언트 IP 주소
    virtual InetAddress getLocalAddress() const = 0;

protected:
    Marshaller& getMarshaller() {
        return *marshaller_;
    }

    SessionCallback& getCallback() {
        assert(callback_ != nullptr);
        return *callback_;
    }

    MemoryBlockStreamBuffer& getInputStreamBuffer() {
        return *istreamBuffer_;
    }

    MemoryBlockStreamBuffer& getOutputStreamBuffer() {
        return *ostreamBuffer_;
    }

    core::IStream& getInputStream() {
        return *istream_;
    }

    core::OStream& getOutputStream() {
        return *ostream_;
    }

    SessionStats& stats() {
        return stats_;
    }

    size_t getMaxPendablePacketCount() const {
        return maxPendablePacketCount_;
    }

private:
    void initStream(core::StreamFactory& streamFactory);

private:
    const uint16_t maxPendablePacketCount_;

    SessionDestroyer& sessionDestroyer_;
    SessionCallback* callback_;

    std::unique_ptr<MemoryBlockStreamBuffer> istreamBuffer_;
    std::unique_ptr<MemoryBlockStreamBuffer> ostreamBuffer_;
    std::unique_ptr<core::IStream>  istream_;
    std::unique_ptr<core::OStream> ostream_;
    std::unique_ptr<Marshaller> marshaller_;

    SessionStats stats_;

    ExtensionMap extensionMap_;
};

}} // namespace sne { namespace base {
