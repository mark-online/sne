#pragma once

#include "../s2s/S2sCertificate.h"
#include <sne/sgp/protocol/cs/CsMessage.h>
#include <sne/core/memory/MemoryPoolMixin.h>

namespace sne { namespace server {

/** @addtogroup sgp
* @{
*/

/// S2S ���� �޼��� Ÿ��
enum S2sMessageType
{
    // = system messages
    s2smtSystem_authenticate = sgp::csmtSystemLast,
    s2smtSystem_authenticateResult,
    s2smtSystemLast
};


/**
 * @struct S2sAuthenticateMessage
 */
struct S2sAuthenticateMessage : base::AbstractMessage<s2smtSystem_authenticate>,
    public core::ThreadSafeMemoryPoolMixin<S2sAuthenticateMessage>
{
    S2sCertificate certificate_;

    S2sAuthenticateMessage(const S2sCertificate& certificate = "") :
        certificate_(certificate) {}

private:
    virtual void serialize(core::Stream& stream) override {
        stream & certificate_;
    }
};


/**
 * @struct S2sAuthenticateResultMessage
 */
struct S2sAuthenticateResultMessage :
    base::AbstractMessage<s2smtSystem_authenticateResult>,
    public core::ThreadSafeMemoryPoolMixin<S2sAuthenticateResultMessage>
{
    bool authenticated_;

    S2sAuthenticateResultMessage(bool authenticated = false) :
        authenticated_(authenticated) {}

private:
    virtual void serialize(core::Stream& stream) override {
        stream & authenticated_;
    }
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace server {
