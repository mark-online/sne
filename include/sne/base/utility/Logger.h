#pragma once

#include <sne/Common.h>

/** @addtogroup logger
* @{
*/

namespace sne { namespace base {

enum SeverityLevel
{
    trace,
    debug,
    info,
    warning,
    error,
    fatal
};

void SNE_API log(SeverityLevel level, const char* format, ...);
void SNE_API log(const char* function, int line,
    SeverityLevel level, const char* format, ...);

namespace detail {
void enableLogger(bool on);
}

}} // namespace sne { namespace base {

#define SNE_LOG_TRACE(format, ...) \
    sne::base::log(__FUNCTION__, __LINE__, sne::base::trace, format, ## __VA_ARGS__)

#define SNE_LOG_DEBUG(format, ...) \
    sne::base::log(sne::base::debug, format, ## __VA_ARGS__)

#define SNE_LOG_INFO(format, ...) \
    sne::base::log(sne::base::info, format, ## __VA_ARGS__)

#define SNE_LOG_WARNING(format, ...) \
    sne::base::log(sne::base::warning, format, ## __VA_ARGS__)

#define SNE_LOG_ERROR(format, ...) \
    sne::base::log(sne::base::error, format, ## __VA_ARGS__)

#define SNE_LOG_FATAL(format, ...) \
    sne::base::log(sne::base::fatal, format, ## __VA_ARGS__)

/** @} */ // addtogroup logger
