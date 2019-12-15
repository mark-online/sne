#pragma once

#include <sne/sgp/protocol/security/PacketSeedExchanger.h>
#include <sne/base/protocol/Message.h>

namespace sne { namespace sgp {

/**
 * @class PacketSeedExchangerForServer
 */
class PacketSeedExchangerForServer : public PacketSeedExchanger
{
private:
    virtual void registerMessageCallbacks(base::Session& session) override;

    virtual void exchangePublicKey() override;
    virtual void exchangeNextSeed() override;

private:
    /// 초기 시드값을 교환한다
    void exchangeFirstSeed(const std::string& publicKey);

    void exchangeSeed(const std::string& publicKey);

private:
    void onExchangePublicKey(const base::Message& message);
    void onConfirmSeed(const base::Message& message);

private:
    std::string privateKey_;
    std::string otherSidePublicKey_;
    std::string exchangingDecryptSeed_;
};


/**
* @class PacketSeedExchangerForClient
*/
class PacketSeedExchangerForClient : public PacketSeedExchanger
{
private:
    virtual void registerMessageCallbacks(base::Session& session) override;

    virtual void exchangePublicKey() override;
    virtual void exchangeNextSeed() override {
        // NOP
    }

private:
    void onExchangeSeed(const base::Message& message);

private:
    std::string privateKey_;
    std::string otherSidePublicKey_;
};

}} // namespace sne { namespace sgp {
