#pragma once

#include <sne/core/Exception.h>

namespace sne { namespace core {

/** @addtogroup Exception
* @{
*/

/**
 * @class StreamingException
 *
 * 스트리밍(marshaling/unmarshaling) 에러
 */
class StreamingException : public Exception
{
public:
    StreamingException(const char* file, int fileno, const char* what) :
        Exception(file, fileno, what) {}
};

/** @} */ // addtogroup Exception

}} // namespace sne { namespace core
