#pragma once

#include "CsMessage.h"
#include "../sgp.h"

namespace sne { namespace sgp {

/** @addtogroup sgp
* @{
*/

/**
 * @struct CsPacketHeader
 * C/S 구조를 위한 패킷 헤더
 * - header := <body-size> + <message-type>
 */
struct /*SNE_API*/ CsPacketHeader : public AbstractPacketHeader
{
    base::MessageType messageType_;

    explicit CsPacketHeader(BodySize bodySize = 0,
        base::MessageType messageType = base::mtUnknown) :
        AbstractPacketHeader(bodySize),
        messageType_(messageType) {}

    static size_t getHeaderSize() {
        return AbstractPacketHeader::getHeaderSize() + sizeof(uint8_t);
    }

    virtual void reset() override {
        AbstractPacketHeader::reset();
        messageType_ = base::mtUnknown;
    }

    virtual bool isValid() const override {
        if (! AbstractPacketHeader::isValid()) {
            return false;
        }
        return base::isValidMessage(messageType_);
    }

    virtual size_t write(void* buffer) const override {
        assert(base::isValidMessage(messageType_));
        uint8_t* to = static_cast<uint8_t*>(buffer);
        to += AbstractPacketHeader::write(to);
        to += packet::put(static_cast<uint8_t>(messageType_), to);
        return to - static_cast<uint8_t*>(buffer);
    }

    virtual size_t read(const void* buffer) override {
        const uint8_t* from = static_cast<const uint8_t*>(buffer);
        from += AbstractPacketHeader::read(from);
        uint8_t messageType;
        from += packet::get(messageType, from);
        messageType_ = base::toMessageType(messageType);
        return from - static_cast<const uint8_t*>(buffer);
    }
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace sgp {
