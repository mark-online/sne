#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace core {

/** @addtogroup Stream
* @{
*/

/**
 * @class Stream
 *
 * Stream base class
 */
class Stream : public boost::noncopyable
{
public:
    virtual ~Stream() {}

    /// IStream?
    virtual bool isInput() const = 0;

    /// OStream?
    virtual bool isOutput() const = 0;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core
