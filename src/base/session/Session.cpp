#include "BasePCH.h"
#include <sne/base/session/Session.h>
#include <sne/base/session/impl/SessionImpl.h>
#include <sne/base/session/manager/SessionDestroyer.h>

namespace sne { namespace base {

Session::Session()
{
}


Session::~Session()
{
}



void Session::initialize()
{
    if (impl_.get() != nullptr) {
        impl_->open();
    }
}


void Session::finalize()
{
    disconnect();

    if (impl_.get() != nullptr) {
        impl_.reset();
    }
}


void Session::setImplementation(std::unique_ptr<SessionImpl> impl)
{
    assert(! impl_.get());
    impl_.reset(impl.release());
    if (impl_.get() != nullptr) {
        impl_->setCallback(*this);
    }
}


bool Session::connect(const std::string& host, uint16_t port, msec_t timeout)
{
    return impl_->connect(host, port, timeout);
}


void Session::disconnect()
{
    impl_->disconnect();
}


void Session::disconnectGracefully()
{
    impl_->disconnectGracefully();
}


void Session::sendMessage(const void* buffer, size_t bufferSize,
    MessageType messageType)
{
    MemoryBlock* mblock = impl_->serialize(buffer, bufferSize);
    if (mblock != nullptr) {
        impl_->sendMessage(*mblock, messageType);
    }
}


bool Session::isConnected() const
{
    return impl_->isConnected();
}


SessionStats Session::getStats() const
{
    return impl_->getStats();
}


IoContext& Session::getIoContext()
{
    return impl_->getIoContext();
}


SessionExtension* Session::getExtension(SessionExtension::Id extId)
{
    return impl_->getExtension(extId);
}

// = SessionCallback overriding

bool Session::onMessageArrived(MessageType messageType,
    MemoryBlock& mblock)
{
    return impl_->handleMessage(messageType, mblock);
}


void Session::onRelease()
{
    impl_->getSessionDestroyer().release(this);
}

}} // namespace sne { namespace base {
