#pragma once

#include <sne/core/container/Containers.h>

namespace sne { namespace server {

/**
 * @struct SocketAddress
 * IP address + port number
 */
struct SocketAddress
{
    std::string ip_;
    uint16_t port_;

    SocketAddress() :
        port_(0) {}

    SocketAddress(const std::string& ip, uint16_t port) :
        ip_(ip),
        port_(port) {}

    bool operator==(const SocketAddress& rhs) const {
        return (ip_ == rhs.ip_) && (port_ == rhs.port_);
    }

    bool operator<(const SocketAddress& rhs) const {
        if (ip_ < rhs.ip_) {
            return true;
        }
        if (ip_ > rhs.ip_) {
            return false;
        }
        return port_ < rhs.port_;
    }

    void reset() {
        ip_.clear();
        port_ = 0;
    }

    bool isValid() const {
        return (! ip_.empty()) && (port_ > 0);
    }

    template <typename Stream>
    void serialize(Stream& stream) {
        stream & ip_ & port_;
    }
};


typedef core::Vector<SocketAddress> SocketAddresses;

}} // namespace sne { namespace server {
