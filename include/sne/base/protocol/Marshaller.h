#pragma once

#include "Message.h"
#include <boost/noncopyable.hpp>

namespace sne { namespace base {

class MemoryBlock;


/**
* @class MarshallerCallback
*/
class MarshallerCallback
{
public:
    virtual ~MarshallerCallback() {}

    /// 메시지가 unmarshal되었다
    /// @return 메시지를 처리하면 true, 아니면 false 리턴
    virtual bool messageUnmarshalled(MessageType messageType,
        MemoryBlock& mblock) = 0;
};


/**
 * @class Marshaller
 */
class Marshaller : public boost::noncopyable
{
public:
    virtual ~Marshaller() {}

    virtual void reset() = 0;

    virtual void setCallback(MarshallerCallback& callback) = 0;

    virtual bool marshal(MemoryBlock& mblock,
        MessageType messageType) = 0;

    virtual bool unmarshal(size_t& receivedMessageCount) = 0;

    virtual MemoryBlock& getReceivedBlock() = 0;

    virtual MemoryBlock& acquireSendBlock() = 0;
    virtual MemoryBlock& acquireRecvBlock() = 0;
};

}} // namespace sne { namespace base {
