#include "BasePCH.h"
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>
#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include <boost/stacktrace.hpp>

namespace sne { namespace base {

namespace detail
{

void defaultAssertHandler()
{
#ifdef _WIN32
#   ifndef NDEBUG
    DebugBreak();
#   endif
#endif
}

AssertHandler assertHandler = &defaultAssertHandler;

void asserted(const char* what, const char* exp, const char* func,
    const char* filename, unsigned int line_no)
{
    std::ostringstream oss;
    oss << boost::stacktrace::stacktrace();
    SNE_LOG_ERROR("%s FAILED: %s, %s, %s:%d\n%s",
        what, exp, func, filename, line_no, oss.str().c_str());

    if (assertHandler != nullptr) {
        (*assertHandler)();
    }
    else {
        assert(false && __FUNCTION__);
    }
}

} // namespace detail

void setAssertHandler(AssertHandler newHandler)
{
    detail::assertHandler = newHandler;
}

}} // namespace sne { namespace base {
