#pragma once

#include <sne/base/session/impl/SessionImpl.h>
#include <sne/base/io_context/Timer.h>
#include <sne/base/protocol/Marshaller.h>
#include <sne/core/container/Containers.h>
#include <sne/Common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <queue>

namespace sne { namespace base {

class BandwidthLimit;

/** @addtogroup session
* @{
*/

/**
 * @class TcpSessionImpl
 *
 * Asynchronous TCP/IP SessionImpl
 */
class SNE_API TcpSessionImpl :
    public SessionImpl,
    private MarshallerCallback,
    private TimerCallback
{
    using MessageQueue = core::Queue<MemoryBlock*>;

    enum { maxGatherWriteCount = 10 };

public:
    TcpSessionImpl(const SessionConfig& config,
        base::IoContext& ioContext,
        std::unique_ptr<Marshaller> marshaller,
        core::StreamFactory& streamFactory);
    virtual ~TcpSessionImpl() override;

public:
    boost::asio::ip::tcp::socket& getSocket() {
        return socket_;
    }

private:
    virtual void open() override;

    virtual bool connect(const std::string& host, uint16_t port, msec_t timeout) override;

    virtual void disconnect() override;

    /**
     * 클라이언트가 FIN을 받은 후 접속을 해제하지 않으면, 서버가 접속해제를
     * 할 수 없으므로 별도의 접속해제 타이머를 둬서 강제로 접속을 해제한다.
     */
    virtual void disconnectGracefully() override;

    virtual void sendMessage(MemoryBlock& mblock,
        MessageType messageType) override;

private:
    virtual bool isConnected() const override {
        return socket_.is_open();
    }

    virtual IoContext& getIoContext() override;

    virtual InetAddress getRemoteAddress() const override {
        return remoteAddress_;
    }

    virtual InetAddress getLocalAddress() const override {
        return localAddress_;
    }

private: // = MemoryBlockProvider overriding
    virtual MemoryBlock& acquireSendBlock() override {
        return getMarshaller().acquireSendBlock();
    }
    virtual MemoryBlock& acquireRecvBlock() override {
        return getMarshaller().acquireRecvBlock();
    }

private: // = MarshallerCallback overriding
    virtual bool messageUnmarshalled(MessageType messageType,
        MemoryBlock& mblock) override;

private: // = TimerCallback overriding
    virtual void timedOut(const void* act) override;

private: // = boost::asio callback
    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error,
        MemoryBlock* memoryBlock);
    void handle_gather_write(const boost::system::error_code& error);
    //void handle_time_out(const ACE_Time_Value& tv, const void* act);

private:
    void reset();
    void flushOutputQueue();

    bool sessionOpened();
    void setAddresses();

    // @pre 재진입이 안된다
    bool messageBlockReceived(size_t bytesTransferred);

    void readMessage();

    void gatherWrite();
    bool writeMessage(MemoryBlock* mblock);
    void writeSucceeded(MemoryBlock& mblock);

    bool addInboundBandwidth(size_t bytesTransferred);

    void startDisconnectTimer();
    void stopDisconnectTimer();

    void throttleReceiving();
    void startThrottleTimer();
    void stopThrottleTimer();

    bool checkOutputQueue();

    MemoryBlock* popOutputQueue() {
        MemoryBlock* mblock = outputQueue_.front();
        outputQueue_.pop();
        return mblock;
    }

private:
    bool canSendMessage() const {
        return isConnected() && (! shouldBlockWrite());
    }

    bool isDisconnectReserved() const {
        return disconnectTimer_->isRunning();
    }

    bool shouldBlockWrite() const {
        return disconnectTimer_->isRunning();
    }

private:
    boost::asio::ip::tcp::socket socket_;
    MessageQueue outputQueue_;

    InetAddress remoteAddress_;
    InetAddress localAddress_;

    bool isWritePended_;
    size_t prevLoggedOutputQueueSize_;
    core::Vector<boost::asio::const_buffer> outputBuffers_;
    core::Vector<MemoryBlock*> outputMemoryBlocks_;

    std::unique_ptr<base::Timer> disconnectTimer_;

    std::unique_ptr<BandwidthLimit> incomingBandwidthLimiter_;
    std::unique_ptr<base::Timer> throttleTimer_;

    std::chrono::steady_clock::time_point connectedTime_; ///< 접속 시간
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
