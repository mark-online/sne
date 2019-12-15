#pragma once

#include <boost/core/ignore_unused.hpp>

namespace sne { namespace base {

class Session;

/** @addtogroup session
* @{
*/

/**
 * @class SessionCreator
 *
 * 세션을 생성한다.
 */
class SessionCreator
{
public:
    virtual ~SessionCreator() {}

    /// Session 세션을 생성한다.
    virtual Session* acquire() = 0;

    /// Session이 수락되었다(for 테스트)
    virtual void accepted(Session& session) {
        boost::ignore_unused(session);
    }
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
