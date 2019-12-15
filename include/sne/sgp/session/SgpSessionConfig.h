#pragma once

#include "../protocol/PacketCoder.h"
#include "../protocol/security/PacketSeedExchanger.h"
#include <sne/base/session/detail/SessionConfig.h>
#include <sne/base/memory/MemoryBlockManager.h>

namespace sne { namespace core {
class MemoryBlockManager;
}} // namespace sne { namespace core {

namespace sne { namespace sgp {

/** @addtogroup sgp
* @{
*/

/**
* @struct SgpSessionConfig
*/
struct SgpSessionConfig : public base::SessionConfig
{
    base::MemoryBlockManager* memoryBlockManager_;
    PacketCoder* packetCoder_;
    PacketSeedExchanger* packetSeedExchanger_;

    SgpSessionConfig() :
        memoryBlockManager_(nullptr),
        packetCoder_(nullptr),
        packetSeedExchanger_(nullptr) {}

    /**
    * ctor
    * @param messageBlockManager 메모리 블럭 관리자.
    *        메모리 소유권을 넘겨야 한다(동적 할당).
    * @param packetCoder PacketCoder 인스턴스.
    *        메모리 소유권을 넘겨야 한다(동적 할당).
    */
    SgpSessionConfig(std::unique_ptr<base::MemoryBlockManager> memoryBlockManager,
        std::unique_ptr<PacketCoder> packetCoder,
        std::unique_ptr<PacketSeedExchanger> packetSeedExchanger,
        base::SessionDestroyer& sessionDestroyer,
        base::IoContextTask& ioContextTask,
        const base::SessionCapacity& capacity) :
        base::SessionConfig(sessionDestroyer, ioContextTask, capacity),
        memoryBlockManager_(memoryBlockManager.release()),
        packetCoder_(packetCoder.release()),
        packetSeedExchanger_(packetSeedExchanger.release()) {}

    bool isValid() const {
        return base::SessionConfig::isValid() && (memoryBlockManager_ != nullptr) &&
            (packetCoder_ != nullptr) && (packetSeedExchanger_ != nullptr);
    }
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace sgp {
