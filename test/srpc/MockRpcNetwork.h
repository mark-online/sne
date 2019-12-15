#pragma once

#include <sne/srpc/RpcNetwork.h>
#include <sne/srpc/detail/RpcCommand.h>
#include <sne/core/stream/OStream.h>
#include <sne/test/DummyStreamBuffer.h>

using namespace sne;

/**
 * @class MockRpcNetwork
 */
class MockRpcNetwork : public srpc::RpcNetwork
{
public:
    MockRpcNetwork();
    virtual ~MockRpcNetwork();

    void setMarshalingError() {
        buffer_.setPushError();
    }

    core::OStream& getOutputStream() {
        return *ostream_;
    }

    core::StreamBuffer& getStreamBuffer() {
        return buffer_;
    }

    bool sendFailed() const {
        return sendFailed_;
    }

    int getSentRpcHint() const {
        return sentRpcHint_;
    }
private:
    virtual void send(srpc::RpcCommand& command,
        srpc::RpcPacketType packetType, const void* rpcHint) override;
private:
    test::DummyStreamBuffer buffer_;
    std::unique_ptr<core::OStream> ostream_;
    bool sendFailed_;
    int sentRpcHint_;
};
