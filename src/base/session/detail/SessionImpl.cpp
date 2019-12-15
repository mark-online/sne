#include "BasePCH.h"
#include <sne/base/session/impl/SessionImpl.h>
#include <sne/base/session/detail/SessionConfig.h>
#include <sne/base/protocol/Marshaller.h>
#include <sne/base/protocol/MemoryBlockStreamBuffer.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/stream/StreamFactory.h>
#include <sne/core/stream/IStream.h>
#include <sne/core/stream/OStream.h>

namespace sne { namespace base {

SessionImpl::SessionImpl(const SessionConfig& config,
    std::unique_ptr<Marshaller> marshaller,
    core::StreamFactory& streamFactory) :
    maxPendablePacketCount_(config.sessionCapacity_.maxPendablePacketCount_),
    sessionDestroyer_(*config.sessionDestroyer_),
    callback_(nullptr),
    marshaller_(std::move(marshaller))
{
    initStream(streamFactory);
}


SessionImpl::~SessionImpl()
{
}


SessionExtension* SessionImpl::getExtension(SessionExtension::Id extId)
{
    for (auto& value : extensionMap_) {
        const SessionExtension::Id id = value.first;
        if (id == extId) {
            SessionExtension* extension = value.second.get();
            return extension;
        }
    }
    return nullptr;
}


bool SessionImpl::handleMessage(MessageType messageType, MemoryBlock& mblock)
{
    for (auto& value : extensionMap_) {
        SessionExtension* extension = value.second.get();
        if (extension->handleMessage(messageType, mblock)) {
            return true;
        }
    }
    return false;
}


MemoryBlock* SessionImpl::serialize(const void* buffer, size_t bufferSize)
{
    MemoryBlock& mblock = acquireSendBlock();
    mblock.copyFrom(reinterpret_cast<const char*>(buffer), bufferSize);
    return &mblock;
}


MemoryBlock* SessionImpl::serialize(core::Streamable& to)
{
    MemoryBlock& mblock = acquireSendBlock();

    ostream_->reset(false);
    ostreamBuffer_->reset(mblock);

    try {
        to.serialize(*ostream_);
        ostream_->align();
        return &mblock;
    }
    catch (const core::Exception& e) {
        SNE_LOG_DEBUG("SessionImpl::serialize(output) FAILED(%s).",
            e.what());
        mblock.release();
    }

    disconnect();
    return nullptr;
}


core::IStream& SessionImpl::deserialize(MemoryBlock& mblock)
{
    istream_->reset(false);
    istreamBuffer_->reset(mblock);

    return *istream_;
}


void SessionImpl::initStream(core::StreamFactory& streamFactory)
{
    istreamBuffer_ = std::make_unique<MemoryBlockStreamBuffer>();
    istream_ = streamFactory.createIStream(*istreamBuffer_);

    ostreamBuffer_ = std::make_unique<MemoryBlockStreamBuffer>();
    ostream_ = streamFactory.createOStream(*ostreamBuffer_);
}

}} // namespace sne { namespace base {
