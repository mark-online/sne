#include "SgpPCH.h"
#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/utility/CppUtils.h>
#include <boost/core/ignore_unused.hpp>

namespace sne { namespace sgp {

PacketCoder::Seed PacketCoder::generateCipherSeed(uint32_t base) const
{
    srand(base);
    return core::formatString("%u", rand());
}


bool PacketCoder::readHeader(AbstractPacketHeader& header,
    const base::MemoryBlock& mblock) const
{
    //if (! isHeadOfPacket(mblock)) {
    //    return false;
    //}

    if (! isValidPacketHeader(mblock)) {
        return false;
    }

    (void)header.read(mblock.rd_ptr());

    return header.isValid();
}


bool PacketCoder::encode(base::MemoryBlock& mblock,
    AbstractPacketHeader& header)
{
    if (! header.isValid()) {
        return false;
    }

    //if (! isHeadOfPacket(mblock)) {
    //    return false;
    //}

    if (! isValidPacket(mblock)) {
        return false;
    }

    (void)header.write(mblock.base());

    return header.isValid();
}


bool PacketCoder::decode(base::MemoryBlock& mblock)
{
    boost::ignore_unused(mblock);
    return true;
    //return isHeadOfPacket(mblock);
}

}} // namespace sne { namespace sgp {
