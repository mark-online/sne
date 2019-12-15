#pragma once

#include "IStream.h"
#include "OStream.h"
#include <boost/noncopyable.hpp>

namespace sne { namespace core {

/** @addtogroup Stream
* @{
*/

/**
 * @class Streamable
 */
class Streamable : public boost::noncopyable
{
public:
    virtual ~Streamable() {}

    /// serializing
    virtual void serialize(Stream& stream) = 0;
};


/// same as boost.stream
template <typename T>
inline Stream& operator&(Stream& stream, T& value)
{
    if (stream.isInput()) {
        operator >>(static_cast<IStream&>(stream), value);
    }
    else {
        operator <<(static_cast<OStream&>(stream), value);
    }
    return stream;
}

/// same as boost.stream
template <typename T>
inline Stream& operator&(Stream& stream, const T& value)
{
    assert(! stream.isInput());
    assert(stream.isOutput());

    operator <<(static_cast<OStream&>(stream), value);
    return stream;
}

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core
