#include "SgpPCH.h"
#include "SgpMarshaller.h"
#include <sne/sgp/session/SgpSessionImplFactory.h>
#include <sne/sgp/session/extension/RpcingExtension.h>
#include <sne/base/session/impl/detail/TcpSessionImpl.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/core/stream/detail/SneStreamFactory.h>

namespace sne { namespace sgp {

std::unique_ptr<base::SessionImpl> SgpSessionImplFactory::create(
    const SgpSessionConfig& config)
{
    std::unique_ptr<core::StreamFactory> streamFactory(
        std::make_unique<core::SneStreamFactory>(config.shouldUseUtf8ForString_,
            config.useBitPacking_));

    base::IoContext& ioContext = config.ioContextTask_->getIoContext();

    std::unique_ptr<base::SessionImpl> sessionImpl(
        std::make_unique<base::TcpSessionImpl>(config,
            ioContext,
            std::make_unique<SgpMarshaller>(config),
            *streamFactory));

    sessionImpl->registerExtension<base::MessagingExtension>();
    sessionImpl->registerExtension<sgp::RpcingExtension>();
    return sessionImpl;
}

}} // namespace sne { namespace sgp {
