#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace core {

class StreamBuffer;
class IStream;
class OStream;

/** @addtogroup Stream
* @{
*/

/**
 * @class StreamFactory
 *
 * IStream, OStream Factory
 */
class StreamFactory : public boost::noncopyable
{
public:
    virtual ~StreamFactory() {}

    /// 입력 스트림을 생성한다
    virtual std::unique_ptr<IStream> createIStream(StreamBuffer& streamBuffer) = 0;

    /// 출력 스트림을 생성한다
    virtual std::unique_ptr<OStream> createOStream(StreamBuffer& streamBuffer) = 0;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core {
