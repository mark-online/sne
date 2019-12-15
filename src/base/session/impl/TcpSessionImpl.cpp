#include "BasePCH.h"
#include "../BandwidthLimit.h"
#include <sne/base/session/impl/detail/TcpSessionImpl.h>
#include <sne/base/session/detail/SessionConfig.h>
#include <sne/base/io_context/IoContext.h>
#include <sne/base/memory/MemoryBlock.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/SystemUtil.h>
#include <sne/core/utility/SystemUtil.h>
#include <boost/core/ignore_unused.hpp>

namespace sne { namespace base {

TcpSessionImpl::TcpSessionImpl(const SessionConfig& config,
    base::IoContext& ioContext,
    std::unique_ptr<Marshaller> marshaller, core::StreamFactory& streamFactory) :
    SessionImpl(config, std::move(marshaller), streamFactory),
    socket_(ioContext),
    disconnectTimer_(ioContext.createTimer(*this)),
    incomingBandwidthLimiter_(std::make_unique<BandwidthLimit>(config.sessionCapacity_)),
    throttleTimer_(ioContext.createTimer(*this))
{
    reset();
}


TcpSessionImpl::~TcpSessionImpl()
{
}


void TcpSessionImpl::open()
{
    sessionOpened();
}


bool TcpSessionImpl::connect(const std::string& host, uint16_t port, msec_t timeout)
{
    boost::ignore_unused(timeout); // TODO: timeout support

    using namespace boost::asio;
    boost::system::error_code error;
    ip::tcp::resolver resolver(getIoContext());
    ip::tcp::resolver::query query(host, "0");
    ip::tcp::endpoint ep = *resolver.resolve(query, error);
    ep.port(port);

    socket_.connect(ep, error);
    if (error) {
        SNE_LOG_ERROR("TcpSessionImpl(0x%p) - Connection Failed(%d,%s).",
            this, error.value(), error.message().c_str());
        return false;
    }

    sessionOpened();

    return isConnected();
}


void TcpSessionImpl::disconnect()
{
    getIoContext().post([this]() {
        if (isDisconnectReserved()) {
            return;
        }

        const bool connected = isConnected();

        stopDisconnectTimer();
        stopThrottleTimer();

        socket_.close();

        if (!connected) {
            return;
        }

        const msec_t duration = core::getElapsedTime(connectedTime_);

        SNE_LOG_DEBUG(
            "TcpSessionImpl(0x%p) Stats: %ums, sent(%u/%u), recv(%u/%u), %u used.",
            this,
            duration,
            stats().sentBytes_, stats().sentMessageCount_,
            stats().recvBytes_, stats().recvMessageCount_,
            stats().useCount_);

        reset();

        getCallback().onDisconnected();

        getCallback().onRelease();
    });
}


void TcpSessionImpl::disconnectGracefully()
{
    getIoContext().post([this]() {
        if (!isDisconnectReserved()) {
            startDisconnectTimer();
        }
    });
}


void TcpSessionImpl::sendMessage(MemoryBlock& mblock,
    MessageType messageType)
{
    getIoContext().post([this, &mblock, messageType]() {
        MemoryBlockGuard block(mblock);

        if (!canSendMessage()) {
            return;
        }

        bool isSucceeded = false;
        bool shouldDisconnectNow = false;

        if (getMarshaller().marshal(*block, messageType)) {
            if (isWritePended_) {
                if (checkOutputQueue()) {
                    outputQueue_.push(block.release());
                    isSucceeded = true;
                }
                else {
                    shouldDisconnectNow = true;
                }
            }
            else {
                if (writeMessage(block.release())) {
                    isSucceeded = true;
                }
            }
        }

        if (isSucceeded) {
            getCallback().onMessageTransfered(messageType);
            block.release();
        }
        else {
            if (shouldDisconnectNow) {
                disconnect();
            }
            else {
                disconnectGracefully();
            }
        }
    });
}


IoContext& TcpSessionImpl::getIoContext()
{
    return static_cast<IoContext&>(socket_.get_io_context());
}


bool TcpSessionImpl::sessionOpened()
{
    disableNagleAlgorithm(socket_);
    setSocketBufferSize(socket_);
    setTcpLoopbackFastPath(socket_);

    setAddresses();

    getMarshaller().setCallback(*this);

    ++stats().useCount_;
    connectedTime_ = std::chrono::steady_clock::now();

    if (!getCallback().onConnected()) {
        disconnect();
        return false;
    }

    readMessage();

    SNE_LOG_DEBUG("TcpSessionImpl(0x%p) opened.", this);
    return true;
}


void TcpSessionImpl::setAddresses()
{
    const boost::asio::ip::tcp::endpoint remote_ep = socket_.remote_endpoint();
    remoteAddress_.address_ = remote_ep.address().to_string();
    remoteAddress_.port_ = remote_ep.port();

    const boost::asio::ip::tcp::endpoint local_ep = socket_.local_endpoint();
    localAddress_.address_ = local_ep.address().to_string();
    localAddress_.port_ = local_ep.port();
}


bool TcpSessionImpl::messageBlockReceived(size_t bytesTransferred)
{
    MemoryBlock& recvBlock = getMarshaller().acquireRecvBlock();
    recvBlock.wr_ptr(bytesTransferred);

    stats().recvBytes_ += bytesTransferred;

    const bool isKarmaRemained = addInboundBandwidth(bytesTransferred);

    size_t receivedMessageCount = 0;
    const bool isSucceeded = getMarshaller().unmarshal(receivedMessageCount);
    stats().recvMessageCount_ += receivedMessageCount;

    if (! isSucceeded) {
        return false;
    }

    if (! isKarmaRemained) {
        throttleReceiving();
        return true;
    }

    readMessage();
    return true;
}


void TcpSessionImpl::readMessage()
{
    MemoryBlock& recvBlock = getMarshaller().acquireRecvBlock();

    const size_t spare = recvBlock.capacity() / 3;
    recvBlock.reserve(spare);

    const size_t recvBytes = recvBlock.space();
    socket_.async_read_some(boost::asio::buffer(recvBlock.wr_ptr(), recvBytes),
        boost::bind(&TcpSessionImpl::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}


void TcpSessionImpl::gatherWrite()
{
    const size_t outputQueueSize = outputQueue_.size();

    if (outputQueueSize <= 0) {
        return;
    }

    if (outputQueueSize >= 2) {
        outputBuffers_.clear();
        outputMemoryBlocks_.clear();
        while (!outputQueue_.empty()) {
            MemoryBlock* mblock = popOutputQueue();
            outputBuffers_.emplace_back(mblock->rd_ptr(), mblock->length());
            outputMemoryBlocks_.push_back(mblock);

            if (outputMemoryBlocks_.size() >= maxGatherWriteCount) {
                break;
            }
        }

        boost::asio::async_write(socket_,
            outputBuffers_,
            boost::bind(&TcpSessionImpl::handle_gather_write, this,
                boost::asio::placeholders::error));

        isWritePended_ = true;
    }
    else {
        MemoryBlock* mblock = popOutputQueue();
        writeMessage(mblock);
    }
}


bool TcpSessionImpl::writeMessage(MemoryBlock* mblock)
{
    assert(mblock != nullptr);

    boost::asio::async_write(socket_,
        boost::asio::buffer(mblock->rd_ptr(), mblock->length()),
        boost::bind(&TcpSessionImpl::handle_write, this,
            boost::asio::placeholders::error, mblock));

    isWritePended_ = true;
    return true;
}


void TcpSessionImpl::writeSucceeded(MemoryBlock& mblock)
{
    isWritePended_ = false;
    ++stats().sentMessageCount_;
    stats().sentBytes_ += mblock.length();
}


void TcpSessionImpl::reset()
{
    socket_.close();
    flushOutputQueue();
    stats().reset();
    isWritePended_ = false;
    prevLoggedOutputQueueSize_ = 0;
    outputBuffers_.clear();
    outputMemoryBlocks_.clear();
    if (disconnectTimer_) {
        disconnectTimer_->cancel();
    }
    if (throttleTimer_) {
        throttleTimer_->cancel();
    }
    getMarshaller().reset();
    incomingBandwidthLimiter_->reset();
}


void TcpSessionImpl::flushOutputQueue()
{
    while (! outputQueue_.empty()) {
        MemoryBlock* mblock = outputQueue_.front();
        mblock->release();
        outputQueue_.pop();
    }
}


void TcpSessionImpl::startDisconnectTimer()
{
    assert(! disconnectTimer_->isRunning());

    const size_t disconnectTimeout = 500;

    disconnectTimer_->once(disconnectTimeout, disconnectTimer_.get());
}


void TcpSessionImpl::stopDisconnectTimer()
{
    disconnectTimer_->cancel();
}


void TcpSessionImpl::throttleReceiving()
{
    startThrottleTimer();

    SNE_LOG_INFO("TcpSessionImpl(0x%p) - Client(%s:%d) is THROTTLED(%d > %d), "
        "delaying read(%d sec).",
        this,
        remoteAddress_.address_.c_str(), remoteAddress_.port_,
        incomingBandwidthLimiter_->getReadBytes(),
        incomingBandwidthLimiter_->getMaxBytesPerSecond(),
        incomingBandwidthLimiter_->getSecondsForThrottling());

    getCallback().onThrottling(incomingBandwidthLimiter_->getReadBytes(),
        incomingBandwidthLimiter_->getMaxBytesPerSecond());
}


void TcpSessionImpl::startThrottleTimer()
{
    if (throttleTimer_->isRunning()) {
        return;
    }

    const msec_t throttleTimeout =
        incomingBandwidthLimiter_->getSecondsForThrottling() * 1000;

    throttleTimer_->once(throttleTimeout, throttleTimer_.get());
}


void TcpSessionImpl::stopThrottleTimer()
{
    throttleTimer_->cancel();
}


bool TcpSessionImpl::checkOutputQueue()
{
    const int logThreshold = 10;

    const size_t queueSize = outputQueue_.size();

    if ((getMaxPendablePacketCount() > 0) &&
        (queueSize >= getMaxPendablePacketCount())) {
        SNE_LOG_WARNING(
            "TcpSessionImpl(0x%p): will disconnect - Too many output queued(%d).",
            this, queueSize);
        return false;
    }

    if (queueSize > (prevLoggedOutputQueueSize_ + logThreshold)) {
        SNE_LOG_INFO("TcpSessionImpl(0x%p): %u output queued.",
            this, queueSize);
        prevLoggedOutputQueueSize_ = queueSize;
    }

    return true;
}


bool TcpSessionImpl::addInboundBandwidth(size_t bytesTransferred)
{
    incomingBandwidthLimiter_->add(bytesTransferred);
    const bool isKarmaRemained = incomingBandwidthLimiter_->check();
    if (! isKarmaRemained) {
        SNE_LOG_DEBUG("TcpSessionImpl(0x%p) - BandwidthLimit left bytes = %d(-%d)",
            this, incomingBandwidthLimiter_->getLeftBytes(), bytesTransferred);
    }
    return isKarmaRemained;
}

// = MarshallerCallback overriding

bool TcpSessionImpl::messageUnmarshalled(MessageType messageType,
    MemoryBlock& mblock)
{
    if (isDisconnectReserved()) {
        return true;
    }

    return getCallback().onMessageArrived(messageType, mblock);
}

// = TimerCallback overriding

void TcpSessionImpl::timedOut(const void* act)
{
    if (act == disconnectTimer_.get()) {
        {
            if (isWritePended_ || (! outputQueue_.empty())) {
                startDisconnectTimer();
                return;
            }
        }
        disconnect();
    }
    else if (act == throttleTimer_.get()) {
        {
            incomingBandwidthLimiter_->reset();
        }

        readMessage();
    }
    else {
        assert(false && "unknown timer");
    }
}

// = boost::asio callback

void TcpSessionImpl::handle_read(const boost::system::error_code& error,
    size_t bytesTransferred)
{
    if (!error) {
        if (messageBlockReceived(bytesTransferred)) {
            return; // success
        }
    }

    const bool isCancelled = (error == boost::asio::error::operation_aborted);
    if (!isCancelled) {
        SNE_LOG_DEBUG("TcpSessionImpl(0x%p)::handle_read() - FAILED(%d,%s).",
            this, error.value(), error.message().c_str());
    }

    disconnect();
}


void TcpSessionImpl::handle_write(const boost::system::error_code& error,
    MemoryBlock* memoryBlock)
{
    MemoryBlockGuard mblock(memoryBlock);

    const bool isSucceeded = !error;
    const bool isCancelled = (error == boost::asio::error::operation_aborted);

    if (isSucceeded) {
        writeSucceeded(*mblock);

        gatherWrite();
    }
    else {
        if (! isCancelled) {
            SNE_LOG_DEBUG(
                "TcpSessionImpl(0x%p)::handle_write() - FAILED(%d,%s).",
                this, error.value(), error.message().c_str());
        }

        disconnect();
    }
}


void TcpSessionImpl::handle_gather_write(const boost::system::error_code& error)
{
    const bool isSucceeded = !error;
    const bool isCancelled = (error == boost::asio::error::operation_aborted);

    for (auto memoryBlock : outputMemoryBlocks_) {
        MemoryBlockGuard mblock(memoryBlock);

        if (isSucceeded) {
            writeSucceeded(*mblock);
        }
    }

    outputBuffers_.clear();
    outputMemoryBlocks_.clear();

    if (isSucceeded) {
        gatherWrite();
    }
    else {
        if (! isCancelled) {
            SNE_LOG_DEBUG(
                "TcpSessionImpl(0x%p)::handle_gather_write() - FAILED(%d,%s).",
                this, error.value(), error.message().c_str());
        }
        disconnect();
    }
}

}} // namespace sne { namespace base {
