#pragma once

#include "../StreamFactory.h"

namespace sne { namespace core {

/** @addtogroup Stream
* @{
*/

enum StreamType {
    stBit, ///< 비트 스트림
    stByte ///< 바이트 스트림
};


/**
 * @class SneStreamFactory
 *
 * IStream, OStream Factory for SNE
 */
class SNE_API SneStreamFactory : public StreamFactory
{
public:
    /// @param shouldUseUtf8ForString 문자열을 UTF-8로 전환?
    /// @param useBitPacking 비트 압축 사용?
    SneStreamFactory(bool shouldUseUtf8ForString, bool useBitPacking) :
        shouldUseUtf8ForString_(shouldUseUtf8ForString),
        useBitPacking_(useBitPacking) {}

public:
    virtual std::unique_ptr<IStream> createIStream(
        StreamBuffer& streamBuffer) override;
    virtual std::unique_ptr<OStream> createOStream(
        StreamBuffer& streamBuffer) override;

private:
    bool shouldUseUtf8ForString_;
    bool useBitPacking_;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core {
