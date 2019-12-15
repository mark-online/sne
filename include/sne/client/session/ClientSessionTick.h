#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace base {
class IoContextTask;
class IoContext;
}} // namespace sne { namespace client {

namespace sne { namespace client {

/**
 * @class ClientSessionTick
 * Client Session Ticker
 */
class SNE_API ClientSessionTick : public boost::noncopyable
{
public:
    ClientSessionTick();
    ~ClientSessionTick();

    /// handle events(패킷을 전송하거나 수신한다)
    void tick();

    base::IoContextTask& getIoContextTask() {
        return *ioContextTask_;
    }

private:
    std::unique_ptr<base::IoContextTask> ioContextTask_;
    base::IoContext* ioContext_;
};

}} // namespace sne { namespace client {
