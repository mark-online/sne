#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#include <memory>

namespace sne { namespace sgp {

class PacketCoder;

/** @addtogroup sgp
* @{
*/

/**
 * @class PacketCoderFactory
 * PacketCodeer factory
 * - Abstract Factory Pattern
 */
class SNE_API PacketCoderFactory : public boost::noncopyable
{
public:
    virtual ~PacketCoderFactory() {}

    /**
     * C/S용 PacketCoder를 생성한다.
     */
    virtual std::unique_ptr<PacketCoder> create() const;
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace sgp {
