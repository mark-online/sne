#pragma once

#include <sne/Common.h>

namespace sne { namespace core { namespace fsm {

class Event;

/** @addtogroup Fsm
* @{
*/

/**
 * @class State
 */
template <typename Owner>
class State : public boost::noncopyable
{
public:
    virtual ~State() {}

public:
    virtual void entry(Owner& owner) = 0;
    virtual void exit(Owner& owner) = 0;
    virtual void update(Owner& owner, msec_t diff) = 0;

    virtual bool handleEvent(Owner& owner, Event& event) const = 0;
};

/** @} */ // addtogroup Fsm

}}} // namespace sne { namespace core { namespace fsm {
