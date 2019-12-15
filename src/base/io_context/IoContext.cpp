#include "BasePCH.h"
#include <sne/base/io_context/IoContext.h>
#include "AsioTimer.h"

namespace sne { namespace base {

std::unique_ptr<Timer> IoContext::createTimer(TimerCallback& callback)
{
    return std::make_unique<AsioTimer>(*this, callback);
}

}} // namespace sne { namespace base {
