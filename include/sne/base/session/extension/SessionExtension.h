#pragma once

#include "../../protocol/Message.h"
#include "../../memory/MemoryBlock.h"
#include <sne/core/utility/Hash.h>
#include <boost/core/ignore_unused.hpp>
#include <boost/noncopyable.hpp>

#define SNE_GENERATE_SESSION_EXTENSION_ID(Class) \
    public: \
        static sne::base::SessionExtension::Id getId() { \
            static sne::base::SessionExtension::Id id = \
             static_cast<sne::base::SessionExtension::Id>( \
                    sne::core::hash(# Class)); \
            return id; \
        } \
    private:


namespace sne { namespace base {

class MemoryBlock;
class SessionImpl;

/** @addtogroup session
* @{
*/

/**
 * @class SessionExtension
 */
class SessionExtension : public boost::noncopyable
{
public:
    enum Id {
        eiUnknown = -1
    };
public:
    virtual ~SessionExtension() {}

    virtual void attachedTo(SessionImpl& sessionImpl) = 0;

    /// 수신된 메시지를 처리한다
    /// @return 메시지를 처리했으면 true
    virtual bool handleMessage(MessageType messageType,
        MemoryBlock& mblock) = 0;
};


/**
 * class AbstractSessionExtension
 */
template <typename T>
class AbstractSessionExtension : public SessionExtension
{
    SNE_GENERATE_SESSION_EXTENSION_ID(T);

public:
    typedef AbstractSessionExtension<T> Parent;

public:
    AbstractSessionExtension() :
        sessionImpl_(nullptr) {}

    virtual void attachedTo(SessionImpl& sessionImpl) override {
        sessionImpl_ = &sessionImpl;
    }

    virtual bool handleMessage(MessageType messageType,
        MemoryBlock& mblock) override {
        boost::ignore_unused(messageType);
        boost::ignore_unused(mblock);
        return false;
    }

protected:
    SessionImpl& getSessionImpl() {
        assert(sessionImpl_ != nullptr);
        return *sessionImpl_;
    }

private:
    SessionImpl* sessionImpl_;
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {

namespace std {

    template <>
    struct hash<sne::base::SessionExtension::Id>
    {
        size_t operator()(sne::base::SessionExtension::Id v) const {
            return static_cast<size_t>(v);
        }
    };

} // namespace std

