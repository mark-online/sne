#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace core { namespace fsm {

/** @addtogroup Fsm
* @{
*/

/**
 * @class Event
 */
class Event : public boost::noncopyable
{
public:
    virtual ~Event() {}
};

/** @} */ // addtogroup Fsm

}}} // namespace sne { namespace core { namespace fsm {
