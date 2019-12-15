#ifndef SNE_PROXYSCHEDULER_H
#define SNE_PROXYSCHEDULER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "RequestMethod.h"
#include <sne/database/proxy/DatabaseRequest.h>
#include <sne/core/container/Containers.h>
#include <boost/core/noncopyable.hpp>
#include <mutex>
#include <queue>
#include <deque>
#include <ctime>

namespace boost {
class thread_group;
}

namespace sne { namespace database {
class DatabaseManager;
}} // namespace sne { namespace database {

namespace sne { namespace database { namespace proxy {

/**
 * @class ProxyScheduler
 *
 * 대행 처리 요청을 처리하기 위한 Task scheduler
 * - Active Object Pattern
 */
class ProxyScheduler : public boost::noncopyable
{
    typedef core::PriorityQueue<RequestMethod*> RequestQueue;
public:
    ProxyScheduler(DatabaseManager& proxyDatabaseManager);
    virtual ~ProxyScheduler();

    bool start(size_t threadCount);
    void stop();

    void schedule(const ModifyRequest& request);

    uint64_t getQueueCount() const;

    uint64_t getRequestedCount() const {
        return queuedCount_;
    }

    uint64_t getProcessedCount() const {
        return processedCount_;
    }

private:
    RequestMethod* fetchRequest();

private:
    // = ACE_Task_Base overring
    virtual int svc();

private:
    DatabaseManager& proxyDatabaseManager_;
    RequestQueue requestQueue_;
    RequestId requestId_;
    volatile bool stop_;

    uint64_t queuedCount_;
    uint64_t processedCount_;

    std::unique_ptr<boost::thread_group> threads_;
    mutable std::mutex mutex_;
    std::condition_variable available_;
};

}}} // namespace sne { namespace database { namespace proxy {

#endif // SNE_PROXYSCHEDULER_H
