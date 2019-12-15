#pragma once

#include <sne/sgp/protocol/cs/CsMessage.h>
#include <sne/core/memory/MemoryPoolMixin.h>

using namespace sne;

/// Echo 메세지 타입
enum EchoMessageType
{
    // = user defined messages
    emtEcho = sgp::csmtUserDefinedLast
};


/**
 * @struct EchoMessage
 */
struct EchoMessage : base::AbstractMessage<emtEcho>,
    public core::ThreadSafeMemoryPoolMixin<EchoMessage>
{
    std::string body_;

    EchoMessage() {}

    EchoMessage(const std::string& body) :
        body_(body) {}

private:
    virtual void serialize(core::Stream& stream) override {
        stream & body_;
    }
};
