#pragma once

#include "SessionExtension.h"
#include <sne/base/protocol/Message.h>
#include <sne/core/container/Containers.h>
#include <sne/Common.h>

namespace sne { namespace base {

/** @addtogroup session
* @{
*/

/**
 * @class MessagingExtension
 *
 * Message 송신/수신 확장
 * - Thread-UNSAFE하므로 Message callback 등록은 객체 초기화 때에 해야 한다
 */
class SNE_API MessagingExtension : public AbstractSessionExtension<MessagingExtension>
{
    SNE_GENERATE_SESSION_EXTENSION_ID(MessagingExtension);

    typedef std::function<void(const Message& message)> MessageCallbackFunc;

    struct MessageCallback {
        Message* message_; /// 성능을 위해 raw pointer 사용
        MessageCallbackFunc func_;

        MessageCallback(Message* message, MessageCallbackFunc func) :
        message_(message),
            func_(func) {}
    };
    typedef core::HashMap<base::MessageType, MessageCallback> MessageCallbackMap;

public:
    virtual ~MessagingExtension();

    /// Messsage Callback을 등록한다
    template <typename MessageT, typename SignatureT, typename HostClassT>
    void registerMessageCallback(SignatureT HostClassT::*mfunc,
        HostClassT* hostClass) {
        const MessageType mt = base::toMessageType(MessageT::type);
        assert(base::isValidMessage(mt));
        assert(messageCallbackMap_.find(mt) == messageCallbackMap_.end());

        MessageCallback callback(
            new MessageT, boost::bind(mfunc, hostClass, _1));
        messageCallbackMap_.emplace(mt, callback);
    }

public:
    void sendMessage(Message& message);

private:
    virtual bool handleMessage(MessageType messageType,
        MemoryBlock& mblock) override;

private:
    MessageCallbackMap messageCallbackMap_;
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
