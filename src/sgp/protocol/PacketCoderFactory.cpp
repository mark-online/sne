#include "SgpPCH.h"
#include "cs/CsPacketCoder.h"
#include <sne/sgp/protocol/PacketCoderFactory.h>

namespace sne { namespace sgp {

std::unique_ptr<PacketCoder> PacketCoderFactory::create() const
{
    return std::make_unique<CsPacketCoder>();
}

}} // namespace sne { namespace sgp {
