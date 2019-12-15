#include "SecurityPCH.h"
#include <sne/security/cipher/RsaCipher.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4100 4189 4244 4267 4702 4706)
#endif
#include <cryptopp/rsa.h>
#include <cryptopp/randpool.h>
#include <cryptopp/filters.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/noncopyable.hpp>

using CryptoPP::byte;

namespace sne { namespace security {

namespace detail {

/**
* @class RsaCipherImpl
*
* RSA cipher implementation
* - https://www.cryptopp.com/wiki/Keys_and_Formats
*/
class RsaCipherImpl : public boost::noncopyable
{
    typedef CryptoPP::RSAES_OAEP_SHA_Decryptor Decryptor;
    typedef CryptoPP::RSAES_OAEP_SHA_Encryptor Encryptor;
    typedef CryptoPP::RSASSA_PKCS1v15_SHA_Signer Signer;
    typedef CryptoPP::RSASSA_PKCS1v15_SHA_Verifier Verifier;

public:
    RsaCipherImpl() {
        initRandomPool(RsaCipher::Seed());
    }

    RsaCipherImpl(size_t privateKeyLength, const RsaCipher::Seed& seed) {
        initRandomPool(seed);

        std::string privKey;
        Decryptor privDecryptor(randomPool_, privateKeyLength);
        CryptoPP::StringSink privKeySink(privKey);
        privDecryptor.AccessKey().DEREncode(privKeySink);
        privKeySink.MessageEnd();

        std::string pubKey;
        Encryptor pubEncryptor(privDecryptor);
        CryptoPP::StringSink pubKeySink(pubKey);
        pubEncryptor.AccessKey().DEREncode(pubKeySink);
        pubKeySink.MessageEnd();

        privateKey_.assign(privKey.c_str(), privKey.length());
        publicKey_.assign(pubKey.c_str(), pubKey.length());
    }

    RsaCipherImpl(const std::string& publicKey, const std::string& privateKey,
        const RsaCipher::Seed& seed) {
        initRandomPool(seed);
        publicKey_ = publicKey;
        privateKey_ = privateKey;
    }

    size_t encrypt(const void* message, size_t messageLength) {
        return encryptOtherPublic(message, messageLength, publicKey_);
    }

    size_t encryptOtherPublic(const void* message, size_t messageLength,
        const std::string& otherPublicKey) {
        CryptoPP::StringSource key(otherPublicKey, true);
        Encryptor encryptor(key);

        assert(messageLength <= encryptor.FixedMaxPlaintextLength());
        assert(encryptor.FixedCiphertextLength() <=
            RsaCipher::maxBufferLength);

        encryptor.Encrypt(randomPool_,
            static_cast<const byte*>(message), messageLength, outputBuffer_);
        return encryptor.FixedCiphertextLength();
    }

    size_t decrypt(const void* encrypted, size_t encryptedLength) {
        CryptoPP::StringSource key(privateKey_, true);
        Decryptor decryptor(key);

        assert(decryptor.FixedMaxPlaintextLength() <=
            RsaCipher::maxBufferLength);

        if (encryptedLength < decryptor.FixedCiphertextLength()) {
            return 0;
        }

        CryptoPP::DecodingResult result =
            decryptor.Decrypt(randomPool_, static_cast<const byte*>(encrypted),
                decryptor.FixedCiphertextLength(), outputBuffer_);

        if (! result.isValidCoding) {
            return 0;
        }

        return result.messageLength;
    }

    size_t sign(const void* message, size_t messageLength) {
        CryptoPP::StringSource key(privateKey_, true);
        Signer signer(key);

        CryptoPP::SignerFilter* filter =
            new CryptoPP::SignerFilter(randomPool_, signer,
                new CryptoPP::ArraySink(outputBuffer_,
                    RsaCipher::maxBufferLength));

        CryptoPP::StringSource source(static_cast<const byte*>(message),
            messageLength, true, filter);
        return signer.SignatureLength();
    }

    bool verify(const void* message, size_t messageLength,
        const void* signature, size_t signatureLength) {
        return verifyOtherPublic(message, messageLength,
            signature, signatureLength, publicKey_);
    }


    bool verifyOtherPublic(const void* message, size_t messageLength,
        const void* signature, size_t signatureLength,
        const std::string& otherPublicKey) {
        CryptoPP::StringSource key(otherPublicKey, true);
        Verifier verifier(key);
        if (signatureLength != verifier.SignatureLength()) {
            return false;
        }

        byte result = 0;
        CryptoPP::SignatureVerificationFilter* filter =
            new CryptoPP::SignatureVerificationFilter(verifier,
                new CryptoPP::ArraySink(&result, 1));
        filter->Put(static_cast<const byte*>(signature),
            verifier.SignatureLength());

        CryptoPP::StringSource source(static_cast<const byte*>(message),
            messageLength, true, filter);
        return result == 1;
    }

    const std::string& getPublicKey() const {
        return publicKey_;
    }

    const std::string& getPrivateKey() const {
        return privateKey_;
    }

    const uint8_t* getOutputBuffer() const {
        return outputBuffer_;
    }

private:
    void initRandomPool(const RsaCipher::Seed& seed) {
        randomPool_.IncorporateEntropy(
            reinterpret_cast<const byte *>(seed.c_str()), seed.length());
    }

private:
    CryptoPP::RandomPool randomPool_;
    std::string publicKey_;
    std::string privateKey_;
    uint8_t outputBuffer_[RsaCipher::maxBufferLength];
};

} // namespace detail

// = RsaCipher

RsaCipher::RsaCipher() :
    impl_(std::make_unique<detail::RsaCipherImpl>())
{
}


RsaCipher::RsaCipher(size_t privateKeyLength, const Seed& seed) :
    impl_(std::make_unique<detail::RsaCipherImpl>(privateKeyLength, seed))
{
}


RsaCipher::RsaCipher(const std::string& publicKey,
    const std::string& privateKey, const Seed& seed) :
    impl_(std::make_unique<detail::RsaCipherImpl>(publicKey, privateKey, seed))
{
}


RsaCipher::~RsaCipher()
{
}


size_t RsaCipher::encrypt(const void* message, size_t messageLength)
{
    try {
        return impl_->encrypt(message, messageLength);
    }
    catch (const std::exception& e) {
        lastErrorMessage_ = e.what();
    }
    return 0;
}


size_t RsaCipher::encryptOtherPublic(const void* message, size_t messageLength,
    const std::string& otherPublicKey)
{
    try {
        return impl_->encryptOtherPublic(message, messageLength, otherPublicKey);
    }
    catch (const std::exception& e) {
        lastErrorMessage_ = e.what();
    }
    return 0;
}


size_t RsaCipher::decrypt(const void* encrypted, size_t encryptedLength)
{
    try {
        return impl_->decrypt(encrypted, encryptedLength);
    }
    catch (const std::exception& e) {
        lastErrorMessage_ = e.what();
    }
    return 0;
}


size_t RsaCipher::sign(const void* message, size_t messageLength)
{
    try {
        return impl_->sign(message, messageLength);
    }
    catch (const std::exception& e) {
        lastErrorMessage_ = e.what();
    }
    return 0;
}


bool RsaCipher::verify(const void* message, size_t messageLength,
    const void* signature, size_t signatureLength)
{
    try {
        return impl_->verify(message, messageLength, signature, signatureLength);
    }
    catch (const std::exception& e) {
        lastErrorMessage_ = e.what();
    }
    return false;
}


bool RsaCipher::verifyOtherPublic(const void* message, size_t messageLength,
    const void* signature, size_t signatureLength,
    const std::string& otherPublicKey)
{
    try {
        return impl_->verifyOtherPublic(message, messageLength,
            signature, signatureLength, otherPublicKey);
    }
    catch (const std::exception& e) {
        lastErrorMessage_ = e.what();
    }
    return false;
}


const std::string& RsaCipher::getPublicKey() const
{
    return impl_->getPublicKey();
}


const std::string& RsaCipher::getPrivateKey() const
{
    return impl_->getPrivateKey();
}


const uint8_t* RsaCipher::getOutputBuffer() const
{
    return impl_->getOutputBuffer();
}


RsaCipher::Seed RsaCipher::generateSeed(size_t length)
{
    //const unsigned int randomValue = time(0);
    //CryptoPP::RandomPool pool;
    //pool.Put(reinterpret_cast<const byte*>(&randomValue), sizeof(randomValue));
    //std::string seed(length, '\0');
    //for (size_t i = 0; i < length; ++i) {
    //    seed[i] = pool.GenerateByte(); // stack이 꼬여버림. 원인 파악 안됨.
    //}
    //return seed;

    std::string seed(length, '\0');
    for (size_t i = 0; i < length; ++i) {
        seed[i] = static_cast<char>(rand());
    }
    return seed;
}

}} // namespace sne { namespace security {
