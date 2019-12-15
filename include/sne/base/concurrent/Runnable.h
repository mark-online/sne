#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace base {

/** @addtogroup Concurrent
* @{
*/

/**
 * @class Runnable
*/
class Runnable : public boost::noncopyable
{
public:
    virtual ~Runnable() {}

    virtual void run() = 0;
};

/** @} */ // addtogroup Concurrent

}} // namespace sne { namespace base {
