#include "SgpPCH.h"
#include "SgpMarshaller.h"
#include <sne/sgp/session/SgpSessionConfig.h>
#include <sne/base/memory/MemoryBlockManager.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/Exception.h>

namespace sne { namespace sgp {

SgpMarshaller::SgpMarshaller(const SgpSessionConfig& config) :
    packetHeaderSize_(config.packetCoder_->getHeaderSize()),
    callback_(nullptr),
    memoryBlockManager_(*config.memoryBlockManager_),
    packetCoder_(config.packetCoder_),
    recvBlock_(memoryBlockManager_.acquire()),
    msgBlock_(memoryBlockManager_.acquire())
{
}


void SgpMarshaller::reset()
{
    packetCoder_->reset();
    headerForReceive_.reset();
    recvBlock_->reset();
    msgBlock_->reset();
}


bool SgpMarshaller::marshal(base::MemoryBlock& mblock, base::MessageType messageType)
{
//#ifndef NDEBUG
//    SNE_LOG_DEBUG("SgpMarshaller::marshal(%d, %d bytes)",
//        messageType, mblock.length());
//#endif

    if (! packetCoder_->isValidPacket(mblock)) {
        SNE_LOG_ERROR("SgpMarshaller::marshal() - Too short message(%d).",
				mblock.length());
        return false;
    }

    CsPacketHeader header(packetCoder_->getBodySize(mblock), messageType);
    if (! packetCoder_->encode(mblock, header)) {
        SNE_LOG_ERROR("SgpMarshaller::marshal() - Packet encoding FAILED.");
        return false;
    }

    return true;
}


bool SgpMarshaller::unmarshal(size_t& receivedMessageCount)
{
    assert(callback_ != nullptr);

    while (isPacketHeaderArrived()) {
        if (! parseHeader()) {
            return false;
        }

        if (! isMessageArrived()) {
            break;
        }

        if (! parseMessage()) {
            return false;
        }

        ++receivedMessageCount;

//#ifndef NDEBUG
//        SNE_LOG_DEBUG("SgpMarshaller::unmarshal(%d)",
//            headerForReceive_.messageType_);
//#endif

        try {
            if (! callback_->messageUnmarshalled(
                headerForReceive_.messageType_, *msgBlock_)) {
                SNE_LOG_ERROR(
                    "SgpMarshaller::unmarshal() - Message(%d) not handled.",
                    headerForReceive_.messageType_);
                return false;
            }
        }
        catch (const core::Exception& e) {
            SNE_LOG_ERROR(
                "SgpMarshaller::unmarshal() - Failed to handle Message(%d)(%s).",
                headerForReceive_.messageType_, e.what());
            return false;
        }

        headerForReceive_.reset();
    }
    return true;
}


base::MemoryBlock& SgpMarshaller::acquireSendBlock()
{
    base::MemoryBlock* mblock = memoryBlockManager_.acquire();
    packetCoder_->reserveHeader(*mblock);
    return *mblock;
}


bool SgpMarshaller::parseHeader()
{
    if (headerForReceive_.isValid()) {
        return true;
    }

    assert(recvBlock_->length() >= packetHeaderSize_);
    if (! packetCoder_->readHeader(headerForReceive_, *recvBlock_)) {
        SNE_LOG_DEBUG("SgpMarshaller::parseHeader() - "
            "Invalid Message Header.");
        return false;
    }

    return true;
}


bool SgpMarshaller::parseMessage()
{
    const size_t messageSize = packetHeaderSize_ + headerForReceive_.bodySize_;

    msgBlock_->reset();
    msgBlock_->size(messageSize);
    msgBlock_->copyFrom(recvBlock_->rd_ptr(), messageSize);

    recvBlock_->rd_ptr(messageSize);
    recvBlock_->crunch();

    if (! packetCoder_->decode(*msgBlock_)) {
        return false;
    }
    packetCoder_->advanceToBody(*msgBlock_);

    if (! isValidMessage(headerForReceive_.messageType_)) {
        SNE_LOG_ERROR("SgpMarshaller::handle_input() - "
            "Invalid Message Type.");
        return false;
    }

    return true;
}

}} // namespace sne { namespace sgp {
