#include "SgpPCH.h"
#include "PacketSeedExchangerImpl.h"
#include <sne/sgp/protocol/security/PacketSeedExchangerCallback.h>
#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/sgp/protocol/cs/CsMessage.h>
#include <sne/base/session/Session.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/utility/Assert.h>

namespace sne { namespace sgp {

// = PacketSeedExchangerForServer

void PacketSeedExchangerForServer::registerMessageCallbacks(
    base::Session& session)
{
    session.getExtension<base::MessagingExtension>()->
        registerMessageCallback<ExchangePublicKeyMessage>(
            &PacketSeedExchangerForServer::onExchangePublicKey, this);

    session.getExtension<base::MessagingExtension>()->
        registerMessageCallback<ConfirmSeedMessage>(
            &PacketSeedExchangerForServer::onConfirmSeed, this);
}


void PacketSeedExchangerForServer::exchangePublicKey()
{
    if (! getPacketCoder().hasPacketCipher()) {
        getCallback().seedExchanged();
    }
}


void PacketSeedExchangerForServer::exchangeNextSeed()
{
    if ((! getPacketCoder().shouldExchangeCipherSeed()) ||
        (! getPacketCoder().isCipherKeyExpired())) {
        return;
    }

    exchangeSeed("");
    getPacketCoder().extendCipherKeyTimeLimit();
}


void PacketSeedExchangerForServer::exchangeFirstSeed(const std::string& publicKey)
{
    if (! getPacketCoder().shouldExchangeCipherSeed()) {
        return;
    }

    exchangingDecryptSeed_.clear();
    exchangeSeed(publicKey);
}


void PacketSeedExchangerForServer::exchangeSeed(const std::string& publicKey)
{
    if (! exchangingDecryptSeed_.empty()) {
        return;
    }

    PacketCoder::Seed seedForEncrypt;
    PacketCoder::Seed seedForDecrypt;
    getPacketCoder().generateCipherSeed(seedForEncrypt, seedForDecrypt);

    exchangingDecryptSeed_ = seedForDecrypt;

    const PacketCoder::Seed seedForEncryptTemp = seedForEncrypt;
    getCallback().encryptSeed(seedForEncrypt, otherSidePublicKey_);
    getCallback().encryptSeed(seedForDecrypt, otherSidePublicKey_);

    ExchangeSeedMessage message;
    message.publicKey_ = publicKey;
    message.seedForEncrypt_ = seedForEncrypt;
    message.seedForDecrypt_ = seedForDecrypt;
    getSession().getExtension<base::MessagingExtension>()->sendMessage(message);

    getPacketCoder().setEncryptSeed(seedForEncryptTemp);
}


void PacketSeedExchangerForServer::onExchangePublicKey(
    const base::Message& message)
{
    const ExchangePublicKeyMessage& theMessage =
        static_cast<const ExchangePublicKeyMessage&>(message);

    if (! getPacketCoder().shouldExchangeCipherSeed()) {
        // 패킷 암호화는 사용하고, 키 교환은 하지 않는 경우
        ExchangeSeedMessage reply;
        getSession().getExtension<base::MessagingExtension>()->sendMessage(reply);
        getCallback().seedExchanged();
        return;
    }

    otherSidePublicKey_ = theMessage.publicKey_;

    std::string myPublicKey;
    if (! getCallback().generatePublicAndPrivateKey(myPublicKey, privateKey_)) {
        SNE_ASSERT(false && "why failed?");
        return;
    }

    exchangeFirstSeed(myPublicKey);
}


void PacketSeedExchangerForServer::onConfirmSeed(
    const base::Message& /*message*/)
{
    assert(getPacketCoder().shouldExchangeCipherSeed());

    getPacketCoder().setDecryptSeed(exchangingDecryptSeed_);
    exchangingDecryptSeed_.clear();

    getCallback().seedExchanged();
}

// = PacketSeedExchangerForClient

void PacketSeedExchangerForClient::registerMessageCallbacks(
    base::Session& session)
{
    session.getExtension<base::MessagingExtension>()->
        registerMessageCallback<ExchangeSeedMessage>(
            &PacketSeedExchangerForClient::onExchangeSeed, this);
}


void PacketSeedExchangerForClient::exchangePublicKey()
{
    if (! getPacketCoder().hasPacketCipher()) {
        getCallback().seedExchanged();
        return;
    }

    std::string publicKey;
    if (! getCallback().generatePublicAndPrivateKey(publicKey, privateKey_)) {
        getCallback().seedExchanged();
        return;
    }

    ExchangePublicKeyMessage message;
    message.publicKey_ = publicKey;
    getSession().getExtension<base::MessagingExtension>()->sendMessage(message);
}


void PacketSeedExchangerForClient::onExchangeSeed(
    const base::Message& message)
{
    const ExchangeSeedMessage& theMessage =
        static_cast<const ExchangeSeedMessage&>(message);

    if (theMessage.isEmpty()) {
        // 패킷 암호화는 사용하고, 키 교환은 하지 않는 경우
        getCallback().seedExchanged();
        return;
    }

    otherSidePublicKey_ = theMessage.publicKey_;

    std::string decryptedSeedForEncrypt = theMessage.seedForEncrypt_;
    if (! getCallback().decryptSeed(decryptedSeedForEncrypt)) {
        SNE_ASSERT(false && "why failed?");
        return;
    }

    std::string decryptedSeedForDecrypt = theMessage.seedForDecrypt_;
    if (! getCallback().decryptSeed(decryptedSeedForDecrypt)) {
        SNE_ASSERT(false && "why failed?");
        return;
    }

    getPacketCoder().setDecryptSeed(decryptedSeedForEncrypt);

    ConfirmSeedMessage replyMessage;
    getSession().getExtension<base::MessagingExtension>()->sendMessage(replyMessage);

    getPacketCoder().setEncryptSeed(decryptedSeedForDecrypt);

    getCallback().seedExchanged();
}

}} // namespace sne { namespace sgp {
