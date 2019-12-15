#pragma once

#include <sne/core/container/Containers.h>
#include <sne/Common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/noncopyable.hpp>
#include <limits>
#include <cassert>

namespace sne { namespace base {

/** @addtogroup utility
* @{
*/

/**
 * @struct InetAddress
 */
struct InetAddress
{
    uint16_t port_;
    std::string address_;

    explicit InetAddress(uint16_t port = 0, const std::string& address = "") :
        port_(port),
        address_(address) {}

    void set(uint16_t port, const std::string& address) {
        port_ = port;
        address_ = address;
    }
};

typedef core::Vector<InetAddress> InetAddresses;


/**
 * sigmin부터 sigmax까지의 모든 시그널을 금지한다.
 * - WIN32 환경에서는 아무런 효과가 없다.
 */
//void SNE_API disableSignals(int sigmin = ACE_SIGRTMIN, int sigmax = ACE_SIGRTMAX);


/// msec만큼 현재 쓰레드를 중지시킨다.
void SNE_API pause(msec_t msec);


/// Disable Nagle alorithm
void SNE_API disableNagleAlgorithm(boost::asio::ip::tcp::socket& socket);


/// 소켓의 송신/수신 버퍼 크기 설정
void SNE_API setSocketBufferSize(boost::asio::ip::tcp::socket& socket,
    size_t bufferSize = (std::numeric_limits<uint16_t>::max)());

/// Using the TCP Loopback Fast Path
/// - https://blogs.technet.microsoft.com/wincat/2012/12/05/fast-tcp-loopback-performance-and-low-latency-with-windows-server-2012-tcp-loopback-fast-path/
void SNE_API setTcpLoopbackFastPath(boost::asio::ip::tcp::socket& socket);

/**
* From: http://support.microsoft.com/kb/263823
* Disable "new behavior" using IOCTL: SIO_UDP_CONNRESET.
* Without this call, recvfrom() can fail, repeatedly,
* after a bad sendto() call.
* -- http://www.gamedev.net/community/forums/topic.asp?topic_id=307848
*/
void SNE_API workaroundWinsockConnectionResetProblem(boost::asio::ip::tcp::socket& socket);

/** @} */ // addtogroup utility

}} // namespace sne { namespace base {
