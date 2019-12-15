#pragma once

namespace sne { namespace base {

class Session;

/** @addtogroup session
* @{
*/

/**
 * @class SessionDestroyer
 *
 * Session 인스턴스를 해제한다.
 */
class SessionDestroyer
{
public:
    virtual ~SessionDestroyer() {}

    /// Session 인스턴스를 사용 후 해제한다.
    virtual void release(Session* session) = 0;
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
