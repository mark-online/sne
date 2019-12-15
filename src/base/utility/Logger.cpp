#include "BasePCH.h"

#ifdef _MSC_VER
#  pragma warning (disable: 4100 4503 4714)
#endif

#include <sne/base/utility/Logger.h>
#include <sne/core/utility/CUtils.h>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED
#include <boost/stacktrace/stacktrace.hpp>
#include <cstdarg>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace sne { namespace base {

namespace detail {

    typedef src::severity_logger_mt<sne::base::SeverityLevel> Logger;
    typedef std::shared_ptr<Logger> LoggerPtr;
    LoggerPtr s_logger;
    bool isEnabled_ = true;

    void enableLogger(bool on)
    {
        isEnabled_ = on;
    }

    inline bool canLog()
    {
        return isEnabled_;
    }

    struct BoostLoggerInitializer
    {
        BoostLoggerInitializer() {
            if (!s_logger) {
                s_logger = std::make_shared<Logger>();
            }
        }
    };

    static BoostLoggerInitializer s_boostLoggerInitializer;
}


void log(SeverityLevel level, const char* format, ...)
{
    if (!detail::canLog()) {
        return;
    }

    const size_t maxlen = 2048;
    char buffer[maxlen+1];

    va_list ap;
    va_start(ap, format);
    const int result = core::vsnprintf(buffer, maxlen, format, ap);
    va_end(ap);

    if (result <= 0) {
        assert(false);
        return;
    }

    BOOST_LOG_SEV(*detail::s_logger, level) << buffer;

    if (level == fatal) {
        std::ostringstream oss;
        oss << boost::stacktrace::stacktrace();
        SNE_LOG_ERROR("StackTrace:\n%s",
            oss.str().c_str());
    }
}


// TODO: 중복 코드 통합
void log(const char* function, int line,
    SeverityLevel level, const char* format, ...)
{
    if (!detail::canLog()) {
        return;
    }

    const size_t maxlen = 2048;
    char buffer[maxlen + 1];

    va_list ap;
    va_start(ap, format);
    const int result = core::vsnprintf(buffer, maxlen, format, ap);
    va_end(ap);

    if (result <= 0) {
        assert(false);
        return;
    }

    BOOST_LOG_SEV(*detail::s_logger, level) <<
        "(" << function << ":" << line << ")" << buffer;
}

}} // namespace sne { namespace base {
