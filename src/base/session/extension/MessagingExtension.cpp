#include "BasePCH.h"
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/session/impl/SessionImpl.h>
#include <sne/base/memory/MemoryBlock.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/Exception.h>

namespace sne { namespace base {

MessagingExtension::~MessagingExtension()
{
    for (auto& value : messageCallbackMap_) {
        boost::checked_delete(value.second.message_);
    }
}


void MessagingExtension::sendMessage(Message& message)
{
    MemoryBlock* mblock = getSessionImpl().serialize(message);
    if (mblock != nullptr) {
        getSessionImpl().sendMessage(*mblock, message.getMessageType());
    }
}


bool MessagingExtension::handleMessage(MessageType messageType,
    MemoryBlock& mblock)
{
    const MessageCallbackMap::iterator pos = messageCallbackMap_.find(messageType);
    if (pos == messageCallbackMap_.end()) {
        return false;
    }

    MessageCallback& callback = (*pos).second;
    core::IStream& istream = getSessionImpl().deserialize(mblock);
    try {
        callback.message_->serialize(istream);
    }
    catch (const core::Exception& e) {
        SNE_LOG_DEBUG("MessagingExtension::handleMessage() FAILED(%s).",
            e.what());
        mblock.release();
        getSessionImpl().disconnect();
        return false;
    }

    callback.func_(*callback.message_);
    return true;
}

}} // namespace sne { namespace base {
