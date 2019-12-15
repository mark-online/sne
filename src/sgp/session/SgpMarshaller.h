#pragma once

#include <sne/sgp/protocol/cs/CsProtocol.h>
#include <sne/base/protocol/Marshaller.h>
#include <sne/base/memory/MemoryBlock.h>

namespace sne { namespace base {
class SessionDestroyer;
class PacketCoder;
class MemoryBlockManager;
}} // namespace sne { namespace base {

namespace sne { namespace sgp {

class PacketCoder;
struct SgpSessionConfig;

/** @addtogroup sgp
* @{
*/

/**
 * @class SgpMarshaller
 */
class SgpMarshaller : public base::Marshaller
{
public:
    SgpMarshaller(const SgpSessionConfig& config);

private:
    virtual void reset() override;

    virtual void setCallback(base::MarshallerCallback& callback) override {
        callback_ = &callback;
    }

    virtual bool marshal(base::MemoryBlock& mblock,
        base::MessageType messageType) override;
    virtual bool unmarshal(size_t& receivedMessageCount) override;

    virtual base::MemoryBlock& getReceivedBlock() override {
        return *msgBlock_;
    }

    virtual base::MemoryBlock& acquireSendBlock() override;
    virtual base::MemoryBlock& acquireRecvBlock() override {
        return *recvBlock_;
    }

private:
    bool parseHeader();
    bool parseMessage();

private:
    bool isPacketHeaderArrived() const {
        return recvBlock_->length() >= packetHeaderSize_;
    }

    bool isMessageArrived() const {
        return recvBlock_->length() >=
            (packetHeaderSize_ + headerForReceive_.bodySize_);
    }

private:
    const size_t packetHeaderSize_;

    base::MarshallerCallback* callback_;

    base::MemoryBlockManager& memoryBlockManager_;
    std::unique_ptr<PacketCoder> packetCoder_;
    CsPacketHeader headerForReceive_;

    base::MemoryBlockGuard recvBlock_;
    base::MemoryBlockGuard msgBlock_;
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace sgp {
