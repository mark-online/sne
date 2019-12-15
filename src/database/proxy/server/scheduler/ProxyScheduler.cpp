#include "DatabasePCH.h"
#include "ProxyScheduler.h"
#include "../ProxyDatabase.h"
#include <sne/database/DatabaseManager.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/utility/Win32Util.h>
#include <boost/thread/detail/thread_group.hpp>

namespace sne { namespace database { namespace proxy {

ProxyScheduler::ProxyScheduler(DatabaseManager& proxyDatabaseManager) :
    proxyDatabaseManager_(proxyDatabaseManager),
    requestId_(invalidRequestId),
    stop_(false),
    queuedCount_(0),
    processedCount_(0)
{
}


ProxyScheduler::~ProxyScheduler()
{
    stop();
}


bool ProxyScheduler::start(size_t threadCount)
{
    assert(! stop_);
    stop_ = false;

    threads_ = std::make_unique<boost::thread_group>();

    for (size_t i = 0; i < threadCount; ++i) {
        if (!threads_->create_thread(std::bind(&ProxyScheduler::svc, this))) {
            return false;
        }
    }
    return true;
}


void ProxyScheduler::stop()
{
    {
        std::unique_lock<std::mutex> lock(mutex_);

        stop_ = true;
        available_.notify_all();
    }

    if (threads_) {
        threads_->join_all();
    }
}


void ProxyScheduler::schedule(const ModifyRequest& request)
{
    size_t queueSize = 0;
    RequestId currentRequestId = invalidRequestId;
    {
        std::unique_lock<std::mutex> lock(mutex_);

        currentRequestId = ++requestId_;
        ModifyRequestMethod* method = new ModifyRequestMethod(proxyDatabaseManager_);
        method->set(currentRequestId, request.priority_);
        method->set(request);
        requestQueue_.push(method);

        ++queuedCount_;
        queueSize = requestQueue_.size();

        available_.notify_all();
    }

    SNE_LOG_INFO("ProxyScheduler::schedule(#%u,%d,%d,%s)[%S](%u)",
        currentRequestId,
        request.priority_, request.type_,
        request.command_.c_str(), request.toString().c_str(),
        queueSize);
}


uint64_t ProxyScheduler::getQueueCount() const
{
    std::unique_lock<std::mutex> lock(mutex_);

    return requestQueue_.size();
}


RequestMethod* ProxyScheduler::fetchRequest()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (requestQueue_.empty()) {
        if (stop_) {
            return nullptr;
        }
        available_.wait(lock);
    }
    
    RequestMethod* requestMethod = requestQueue_.top();
    requestQueue_.pop();
    ++processedCount_;
    return requestMethod;
}


int ProxyScheduler::svc()
{
    core::setThreadName("DB proxy scheduler thread");

    while (! stop_) {
        RequestMethod* requestMethod(fetchRequest());
        if (requestMethod != nullptr) {
            requestMethod->call();
            requestMethod->deleteRequestMethod();
        }
    }

    return 0;
}

}}} // namespace sne { namespace database { namespace proxy {
