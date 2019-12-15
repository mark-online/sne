#include "DatabasePCH.h"
#include <sne/database/proxy/DatabaseClientSideProxy.h>
#include <sne/database/proxy/DatabaseProxyMessage.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace database { namespace proxy {

void DatabaseClientSideProxy::call(const ModifyRequest& request)
{
    assert(request.isValid());

    // TODO: request 메모리 재사용

    SNE_LOG_INFO("DatabaseClientSideProxy::call(%d,%d,%s)[%S]",
        request.priority_, request.type_,
        request.command_.c_str(), request.toString().c_str());

    DbpDelegateMessage message(request);
    sendMessage(message);
}

}}} // namespace sne { namespace database { namespace proxy {
