#pragma once

#include "ServerId.h"
#include "../s2s/S2sCertificate.h"
#include "../common/ServerSpec.h"

namespace sne { namespace server {

enum ServerProperties
{
    maxServerDescLength = 20,
    maxPacketCipherLength = 10
};


/**
 * @struct ServerInfo
 * 서버 정보를 유지한다
 */
struct ServerInfo
{
    ServerId serverId_;
    std::string ip_;
    uint16_t port_;
    S2sCertificate certificate_;
    std::string packetCipher_;
    uint16_t heartbeatInterval_;
    std::string description_;

    ServerInfo() :
        serverId_(ServerId::invalid),
        port_(0),
        heartbeatInterval_(0) {}

    //template <typename Stream>
    //void serialize(Stream& stream) {
    //    stream & serverId_ & description_ & ip_ & port_ & certificate_ &
    //        packetCipher_ & heartbeatInterval_& description_;
    //}
};


inline ServerInfo toServerInfo(const ServerSpec& spec)
{
    ServerInfo serverInfo;
    serverInfo.ip_ = spec.listeningAddresses_[0];
    serverInfo.port_ = spec.listeningPort_;
    serverInfo.certificate_ = spec.certificate_;
    serverInfo.packetCipher_ = spec.packetCipher_;
    serverInfo.heartbeatInterval_ = spec.heartbeatTimeout_ / 3;
    serverInfo.description_ = spec.name_;
    return serverInfo;
}

}} // namespace sne { namespace server {
