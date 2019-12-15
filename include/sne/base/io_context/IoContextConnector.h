#pragma once

#include <sne/Common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>

namespace sne { namespace base {

struct InetAddress;
class IoContextTask;
class SessionCreator;
class SessionDestroyer;

/** @addtogroup io_context
* @{
*/

/**
 * @class IoContextConnector
 * Asynchronous Connector
 */
class SNE_API IoContextConnector : public boost::noncopyable
{
public:
    IoContextConnector(IoContextTask& ioContextTask,
        SessionCreator& sessionCreator, SessionDestroyer& sessionDestroyer);

    /**
     * 서버에 접속을 시도한다.
     * @param serverAddr 연결할 IP 주소.
     * @param connectionCount 연결 횟수.
     */
    bool open(const InetAddress& serverAddr, size_t connectionCount = 1);

    /// Connector를 중지한다.
    void close();

private:
    void connect(size_t connectionIndex);

private:
    IoContextTask& ioContextTask_;
    SessionCreator& sessionCreator_;
    SessionDestroyer& sessionDestroyer_;

    bool shouldFinish_;
    boost::asio::ip::tcp::resolver::results_type endpoints_;
    size_t connectionCount_;
};

/** @} */ // addtogroup io_context

}} // namespace sne { namespace base {
