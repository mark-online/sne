#pragma once

#include <sne/sgp/protocol/PacketCoderFactory.h>
#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/sgp/protocol/sgp.h>
#include <sne/base/utility/SystemUtil.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/utility/Endian.h>
#include <boost/asio.hpp>
#include <boost/core/ignore_unused.hpp>

/**
 * @class TestClient
 *
 * 테스트용 TCP/IP 클라이언트
 */
class TestClient
{
public:
    TestClient() :
        socket_(ioContext_),
        packetCoder_(sne::sgp::PacketCoderFactory().create()) {}
    ~TestClient() {
        close();
    }

    void close() {
        socket_.close();
    }

    // timeout = msec
    bool connect(const std::string& address, uint16_t port) {
        using namespace boost::asio;
        boost::system::error_code error;
        ip::tcp::resolver resolver(ioContext_);
        ip::tcp::resolver::query query(address, "0");
        ip::tcp::endpoint ep = *resolver.resolve(query, error);
        if (error) {
            return false;
        }
        ep.port(port);
        socket_.connect(ep, error);
        return !error;
    }

    bool sendMessage(const uint8_t* body, int bodySize) {
        base::MemoryBlock msg(packetCoder_->getHeaderSize() + bodySize);
        void* wrPtr = msg.wr_ptr();
        *static_cast<sgp::BodySize*>(wrPtr) = core::toRpcByteOrder(
            static_cast<sgp::BodySize>(bodySize));
        *(static_cast<uint8_t*>(wrPtr) + sizeof(sgp::BodySize)) =
            static_cast<uint8_t>(sne::base::mtFirst);
        msg.wr_ptr(packetCoder_->getHeaderSize());
        const void* bodyPtr = body;
        msg.copyFrom(static_cast<const char*>(bodyPtr),
            static_cast<size_t>(bodySize));

        boost::system::error_code error;
        const size_t sentLen = boost::asio::write(socket_,
            boost::asio::buffer(msg.rd_ptr(), msg.length()), error);
        if (error) {
            //printf("sendMessage(%d,%s)\n", error.value(), error.message().c_str());
            return false;
        }
        boost::ignore_unused(sentLen);
        assert(sentLen == msg.length());
        msg.rd_ptr(msg.length());
        return true;
    }

    int recvMessage(void* body, size_t bodySize) {
        sgp::BodySize rpcBodySize = 0;
        boost::system::error_code error;
        boost::asio::read(socket_, boost::asio::buffer(&rpcBodySize, sizeof(rpcBodySize)), error);
        if (error) {
            return -1;
        }

        rpcBodySize = core::toRpcByteOrder(rpcBodySize);
        if (rpcBodySize != static_cast<sgp::BodySize>(bodySize)) {
            return -1;
        }
        uint8_t messageType;
        boost::asio::read(socket_, boost::asio::buffer(&messageType, sizeof(messageType)), error);
        if (error) {
            return -1;
        }
        const int recvLen = static_cast<int>(
            boost::asio::read(socket_, boost::asio::buffer(body, bodySize), error));
        if (error) {
            return -1;
        }
        return recvLen;
    }

private:
    boost::asio::io_context ioContext_;
    boost::asio::ip::tcp::socket socket_;
    std::unique_ptr<sne::sgp::PacketCoder> packetCoder_;
};
