#include "BasePCH.h"
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/utility/SystemUtil.h>
#include <sne/core/utility/Win32Util.h>
#include <sne/base/utility/Logger.h>
#include <boost/thread/thread.hpp>

// https://stackoverflow.com/a/19500405/5642066

namespace sne { namespace base {

IoContextTask::IoContextTask(const std::string& threadName) :
    threadName_(threadName),
    threads_(std::make_unique<boost::thread_group>()),
    nextIoContext_(0)
{
}


IoContextTask::~IoContextTask()
{
}


bool IoContextTask::start(size_t threadCount)
{
    if (threadCount <= 0) {
        throw std::runtime_error("threadCount size is 0");
    }

    for (size_t i = 0; i < threadCount; ++i) {
        IoContextPtr ioContext = std::make_shared<IoContext>();
        ioContexts_.push_back(ioContext);
        works_.push_back(boost::asio::make_work_guard(*ioContext));

        if (!threads_->create_thread(std::bind(&IoContextTask::svc, this, ioContext))) {
            return false;
        }
    }
    return true;
}


void IoContextTask::stop()
{
    for (auto ioContext : ioContexts_) {
        ioContext->stop();
    }
}


void IoContextTask::waitForDone()
{
    threads_->join_all();
}


void IoContextTask::createForTicker()
{
    ioContexts_.push_back(std::make_shared<IoContext>());
}


IoContext& IoContextTask::getIoContext()
{
    // Use a round-robin scheme to choose the next ioContext to use.
    IoContext& ioContext = *ioContexts_[nextIoContext_];
    ++nextIoContext_;
    if (nextIoContext_ == ioContexts_.size()) {
        nextIoContext_ = 0;
    }
    return ioContext;
}


void IoContextTask::svc(IoContextPtr ioContext)
{
#if _WIN32
    core::setThreadName(threadName_);
#endif

    // TODO: disableSignals();

    ioContext->run();
}

}} // namespace sne { namespace base {

