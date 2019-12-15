#include "CorePCH.h"
#include <sne/core/stream/detail/SneStreamFactory.h>
#include <sne/core/stream/detail/IBitStream.h>
#include <sne/core/stream/detail/IByteStream.h>
#include <sne/core/stream/detail/OBitStream.h>
#include <sne/core/stream/detail/OByteStream.h>

namespace sne { namespace core {

namespace {

inline core::StreamType getStreamType(bool useBitPacking)
{
    return useBitPacking ? core::stBit : core::stByte;
}

} // namespace

// = SneStreamFactory

std::unique_ptr<IStream> SneStreamFactory::createIStream(
    StreamBuffer& streamBuffer)
{
    switch (getStreamType(useBitPacking_)) {
    case stBit:
        return std::make_unique<IBitStream>(streamBuffer, shouldUseUtf8ForString_);
    case stByte:
        return std::make_unique<IByteStream>(streamBuffer, shouldUseUtf8ForString_);
    }
    assert(false && "Unknown stream type");
    return std::unique_ptr<IStream>();
}


std::unique_ptr<OStream> SneStreamFactory::createOStream(
    StreamBuffer& streamBuffer)
{
    switch (getStreamType(useBitPacking_)) {
    case stBit:
        return std::make_unique<OBitStream>(streamBuffer, shouldUseUtf8ForString_);
    case stByte:
        return std::make_unique<OByteStream>(streamBuffer, shouldUseUtf8ForString_);
    }
    assert(false && "Unknown stream type");
    return std::unique_ptr<OStream>();
}

}} // namespace sne { namespace core {
