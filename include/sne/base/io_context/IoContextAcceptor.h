#pragma once

#include <sne/Common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>

namespace sne { namespace base {

struct InetAddress;
class IoContextTask;
class SessionCreator;
class SessionDestroyer;
class SessionValidator;
class Session;

/** @addtogroup io_context
* @{
*/

/**
 * @class IoContextAcceptor
 * BOOST ASIO 기반의 Acceptor
 */
class SNE_API IoContextAcceptor : public boost::noncopyable
{
public:
    IoContextAcceptor(IoContextTask& ioContextTask,
        SessionCreator& sessionCreator,
        SessionDestroyer& sessionDestroyer,
        SessionValidator* validator = nullptr);

    /**
     * Acceptor를 초기화하고 accept를 시작한다.
     * @param listenAddr 연결받을 IP 주소.
     * @param backlog -1이면 SOMAXCONN 사용
     * @return 성공 여부.
     */
    bool open(const InetAddress& listenAddr, int backlog = -1);

    /// Acceptor를 닫는다
    void close();

public:
    size_t getAsioAcceptorCount() const {
        return asioAcceptors_.size();
    }

private:
    void accept(boost::asio::ip::tcp::acceptor& acceptor);

private:
    IoContextTask& ioContextTask_;
    SessionCreator& sessionCreator_;
    SessionDestroyer& sessionDestroyer_;
    SessionValidator* validator_;
    bool shouldFinish_;

    std::vector<boost::asio::ip::tcp::acceptor> asioAcceptors_;
};

/** @} */ // addtogroup io_context

}} // namespace sne { namespace base {
