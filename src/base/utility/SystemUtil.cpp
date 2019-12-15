#include "BasePCH.h"
#include <sne/base/utility/SystemUtil.h>
#include <sne/base/utility/Logger.h>
#include <boost/asio.hpp>
#include <boost/core/ignore_unused.hpp>
#include <thread>
#include <cassert>
#if defined(SNE_USE_TCP_LOOPBACK_FAST_PATH)
#   include <mstcpip.h>
#endif

namespace sne { namespace base {

void pause(msec_t msec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}


void disableNagleAlgorithm(boost::asio::ip::tcp::socket& socket)
{
    socket.set_option(boost::asio::ip::tcp::no_delay(true));
}


void setSocketBufferSize(boost::asio::ip::tcp::socket& socket, size_t bufferSize)
{
    socket.set_option(boost::asio::socket_base::send_buffer_size(
        static_cast<int>(bufferSize)));
    socket.set_option(boost::asio::socket_base::receive_buffer_size(
        static_cast<int>(bufferSize)));
}


void setTcpLoopbackFastPath(boost::asio::ip::tcp::socket& socket)
{
#if defined(SNE_USE_TCP_LOOPBACK_FAST_PATH)
    int optionValue = 1;
    DWORD numberOfBytesReturned = 0;

    const int status =
        ::WSAIoctl(socket.native_handle(), SIO_LOOPBACK_FAST_PATH,
            &optionValue, sizeof(optionValue),
            NULL, 0, &numberOfBytesReturned, 0, 0);
    if (SOCKET_ERROR == status) {
        const DWORD lastError = ::GetLastError();
        if (WSAEOPNOTSUPP == lastError) {
            // This system is not Windows 8 or Windows Server 2012, and the call is not supported.
        }
        else {
            SNE_LOG_ERROR("setTcpLookbackFastPath() - Failed to enable SIO_LOOPBACK_FAST_PATH(%d)",
                lastError);
        }
    }
#else
    boost::ignore_unused(socket);
#endif
}


void workaroundWinsockConnectionResetProblem(boost::asio::ip::tcp::socket& socket)
{
#if defined (_WIN32)
   DWORD dwBytesReturned = 0;
   BOOL  bNewBehavior = FALSE; // disable
   const DWORD status = ::WSAIoctl(socket.native_handle(),
       SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0,
       &dwBytesReturned, NULL, NULL);
   if (status == SOCKET_ERROR) {
       SNE_LOG_ERROR("Failed to disable SIO_UDP_CONNRESET(%d)", ::GetLastError());
   }
#else
    boost::ignore_unused(socket);
#endif
}

}} // namespace sne { namespace base {
