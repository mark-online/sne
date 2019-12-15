#include "SrpcPCH.h"
#include <sne/srpc/detail/RpcCommand.h>
#include <sne/srpc/detail/ForwardingFunctors.h>
#include <sne/core/stream/OStream.h>

namespace sne { namespace srpc {

void RpcCommand::marshal(core::OStream& ostream)
{
    ostream.write(rpcId_);
    marshaler_.marshal(ostream);
    ostream.align();
}


void RpcCommand::serialize(core::Stream& stream)
{
    if (stream.isOutput()) {
        marshal(static_cast<core::OStream&>(stream));
    }
    else {
        assert(false && "Output only!");
    }
}

}} // namespace sne { namespace srpc {
