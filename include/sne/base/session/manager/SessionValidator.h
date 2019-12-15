#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace base {

/** @addtogroup session
* @{
*/

/**
 * @class SessionValidator
 */
class SessionValidator : public boost::noncopyable
{
public:
    virtual ~SessionValidator() {}

    /// 세션을 수락할 수 있는가?
    virtual bool isValidated() const = 0;
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
