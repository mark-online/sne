#pragma once

#include <sne/core/stream/Streamable.h>

namespace sne { namespace base {

/** @addtogroup protocol
* @{
*/

/// 메세지 타입. serializing할 때에는 uint8_t(1 바이트)로 처리함
enum MessageType
{
    mtUnknown = -1,

    mtFirst = 0,
    mtLast = 255
};


inline bool isValidMessage(MessageType mt)
{
    return (mtFirst <= mt) && (mt <= mtLast);
}


template <typename T>
inline MessageType toMessageType(T value)
{
    const MessageType mt = static_cast<MessageType>(value);
    assert(isValidMessage(mt));
    return mt;
}


/**
 * @struct Message
 * Network message base class
 */
struct Message : public core::Streamable
{
    virtual MessageType getMessageType() const = 0;
};


/**
 * @struct AbstractMessage
 */
template <int messageType>
struct AbstractMessage : public Message
{
    enum { type = messageType };

private:
    virtual MessageType getMessageType() const override {
        return toMessageType(type);
    }
};

/** @} */ // addtogroup protocol

}} // namespace sne { namespace base {

namespace std {

    template <>
    struct hash<sne::base::MessageType>
    {
        size_t operator()(sne::base::MessageType v) const {
            return static_cast<size_t>(v);
        }
    };

} // namespace std

