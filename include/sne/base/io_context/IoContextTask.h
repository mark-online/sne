#pragma once

#include "IoContext.h"
#include <boost/asio/executor_work_guard.hpp>
#include <boost/noncopyable.hpp>

namespace boost {
class thread_group;
}

namespace sne { namespace base {

/** @addtogroup io_context
* @{
*/

/**
 * @class IoContextTask
 * BOOST ASIO io_context를 구동하기 위한 Thread Pool(io_context per thread).
 * - 인스턴스를 삭제하기 전에 stop()과 waitForDone()를 호출하여 안전하게 삭제할 수
 *   있는 상태까지 대기한 후 삭제해야 한다.
 * - https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/example/cpp03/http/server2/io_context_pool.hpp
 */
class SNE_API IoContextTask : public boost::noncopyable
{
    typedef std::shared_ptr<IoContext> IoContextPtr;
    typedef boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        IoContextWork;

public:
    IoContextTask(const std::string& threadName);
    ~IoContextTask();

    /**
     * @param threadCount event loop를 호출할 쓰레드 갯수.
     */
    bool start(size_t threadCount);

    void stop();

    /// 모든 쓰레드가 종료될 때까지 대기한다.
    void waitForDone();

public:
    /// single thread ticker(ClientnSessionTick)에서 사용
    void createForTicker();

public:
    IoContext& getIoContext();

private:
    void svc(IoContextPtr ioContext);

private:
    const std::string threadName_;
    std::unique_ptr<boost::thread_group> threads_;

    std::vector<IoContextPtr> ioContexts_;
    std::list<IoContextWork> works_;
    std::size_t nextIoContext_;
};

/** @} */ // addtogroup io_context

}} // namespace sne { namespace base {
