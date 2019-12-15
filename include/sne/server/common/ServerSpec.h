#pragma once

#include "../s2s/S2sCertificate.h"
#include "SocketAddress.h"
#include <sne/srpc/RpcContainers.h>
#include <sne/base/session/detail/SessionCapacity.h>

namespace sne { namespace server {

enum ServerSpecProperties
{
    maxListeningAddress = 4,
    maxListeningAddressLength = 30,
};


typedef srpc::RVector<std::string> ListeningAddresses;


inline SocketAddresses toSocketAddresses(const ListeningAddresses& addresses,
    uint16_t port)
{
    SocketAddresses sockets;
    sockets.reserve(addresses.size());

    for (const std::string& address : addresses) {
        if (! address.empty()) {
            sockets.emplace_back(address, port);
        }
    }

    return sockets;
}


/**
 * @struct ServerSpec
 *
 * Server's Session Specification
 */
struct ServerSpec
{
    std::string name_; ///< 식별자

    uint16_t workerThreadCount_; ///< 작업(Proactor) 쓰레드 갯수

    ListeningAddresses listeningAddresses_; ///< 대기 IP 주소들
    uint16_t listeningPort_; ///< 대기 포트 번호

    std::string monitorAddress_; ///< 모니터 서비스(XML-RPC) 대기 주소
    uint16_t monitorPort_; ///< 모니터 서비스(XML-RPC) 대기 포트 번호

    uint16_t sessionPoolSize_; ///< 세션 메모리 풀 크기(> 0)
    uint16_t maxUserCount_; ///< 입장 가능한 최대 사용자 수

    std::string packetCipher_; ///< 패킷 암호화 방법("none", "tea", "aes")

    /// 키 교환 간격 (밀리초 단위).  0이면 사용 안 함.
    uint16_t cipherKeyTimeLimit_;

    /**
     * 인증 유효 시간 (밀리초 단위). 0이면 사용 안 함.
     * - 지정한 시간 이내에 인증하지 않으면 접속을 해제할 것인지 지정한다.
     */
    uint16_t authenticationTimeout_;

    /**
     * 접속 검사 간격 (밀리초 단위). 0이면 사용 안 함.
     * - 클라이언트가 지정한 시간 이내에 접속 유효(heartbeat) 패킷을 보내지
     *   않으면 접속을 해제할 것인지 지정한다.
    */
    uint16_t heartbeatTimeout_;

    /// 세션 용량 관련
    base::SessionCapacity sessionCapacity_;

    /// 인증서(서버 간 연결에만 쓰임)
    S2sCertificate certificate_;

    ServerSpec() :
        workerThreadCount_(0),
        listeningPort_(0),
        monitorPort_(0),
        sessionPoolSize_(0),
        maxUserCount_(0),
        cipherKeyTimeLimit_(0),
        authenticationTimeout_(0),
        heartbeatTimeout_(0),
        sessionCapacity_(base::SessionCapacity::getUnlimitedCapacity()) {}

    bool isValid() const {
        return (! listeningAddresses_.empty()) && (listeningPort_ > 0) &&
            (workerThreadCount_ > 0) && (sessionPoolSize_ > 0) &&
            (! packetCipher_.empty());
    }

    template <typename Stream>
    void serialize(Stream& stream) {
        stream & name_ & workerThreadCount_ &
            listeningAddresses_ & listeningPort_ &
            monitorPort_ & sessionPoolSize_ & maxUserCount_ &
            packetCipher_ & cipherKeyTimeLimit_ &
            authenticationTimeout_ & heartbeatTimeout_ &
            sessionCapacity_.maxBytesPerSecond_ &
            sessionCapacity_.secondsForThrottling_ &
            certificate_;
    }
};

}} // namespace sne { namespace server {
