#pragma once

#include "DatabaseRequest.h"
#include <sne/server/s2s/S2sMessage.h>
#include <sne/core/memory/MemoryPoolMixin.h>

namespace sne { namespace database { namespace proxy {

/** @addtogroup sgp
* @{
*/

/// DatabaseClientSideProxy 전용 메세지 타입
enum DbpMessageType
{
    // = system messages
    dbpmtSystem_delegate = server::s2smtSystemLast,
    dbpmtSystemLast
};


/**
 * @struct DbpDelegateMessage
 */
struct DbpDelegateMessage : base::AbstractMessage<dbpmtSystem_delegate>,
    public core::ThreadSafeMemoryPoolMixin<DbpDelegateMessage>
{
    ModifyRequest request_;

    DbpDelegateMessage() {}
    DbpDelegateMessage(const ModifyRequest& request) :
        request_(request) {}

private:
    virtual void serialize(core::Stream& stream) {
        stream & request_;
    }
};

/** @} */ // addtogroup sgp

}}} // namespace sne { namespace database { namespace proxy {
