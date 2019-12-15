#include "SgpPCH.h"
#include "SecurePacketCoder.h"
#include "CipherKeyGenerator.h"
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>
#include <sne/security/cipher/BlockCipherFactory.h>
#include <sne/core/utility/CppUtils.h>
#include <boost/random/linear_congruential.hpp>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4244 4245 4701 4800)
#endif
#include <boost/crc.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace sne { namespace sgp {

namespace
{

typedef uint16_t FingerPrint;
typedef uint32_t CheckSum;


inline FingerPrint getFingerPrint(security::Key cipherKey)
{
    return static_cast<uint16_t>(cipherKey >> 5);
}


inline CheckSum getCheckSum(const void* first, const void* last,
    FingerPrint fingerPrint)
{
    boost::crc_32_type crc;
    crc.process_block(first, last);
    return crc.checksum() | fingerPrint;
}


/**
* @struct AdditionalPacketHeader
*/
struct AdditionalPacketHeader
{
    enum { headerSize = sizeof(FingerPrint) + sizeof(CheckSum) };

    FingerPrint fingerPrint_;
    CheckSum checkSum_;

    explicit AdditionalPacketHeader(FingerPrint fingerPrint = 0,
        CheckSum checkSum = 0) :
        fingerPrint_(fingerPrint),
        checkSum_(checkSum) {}

    void write(char* to) const {
        to += packet::put(fingerPrint_, to);
        (void)packet::put(checkSum_, to);
    }

    void read(const char* from) {
        from += packet::get(fingerPrint_, from);
        (void)packet::get(checkSum_, from);
    }
};


inline security::Key toCipherKey(const PacketCoder::Seed& seed)
{
    return static_cast<security::Key>(atoi(seed.c_str()));
}

} // namespace

// = SecurePacketCoder

SecurePacketCoder::SecurePacketCoder(std::unique_ptr<PacketCoder> component,
    const std::string& cipher, msec_t cipherKeyPeriod, bool useDynamicKey) :
    component_(std::move(component)),
    cipherKeyGenerator_(CipherKeyGenerator::create(useDynamicKey)),
    cipher_(security::BlockCipherFactory::create(cipher).release()),
    cipherBlockSize_(cipher_->getBlockSize()),
    cipherKeyPeriod_(cipherKeyPeriod),
    rawPacketHeaderSize_(component_->getHeaderSize()),
    securePacketHeaderSize_(
        rawPacketHeaderSize_ + AdditionalPacketHeader::headerSize)
{
}


SecurePacketCoder::~SecurePacketCoder()
{
}


void SecurePacketCoder::reset()
{
    component_->reset();

    cipherKeyGenerator_->reset();

    extendCipherKeyTimeLimit();
}


void SecurePacketCoder::generateCipherSeed(
    Seed& encryptSeed, Seed& decryptSeed) const
{
    boost::rand48 generator(static_cast<boost::int32_t>(clock()));

    encryptSeed = core::formatString("%d", generator());
    decryptSeed = core::formatString("%d", generator());
}


//PacketCoder::Seed SecurePacketCoder::generateCipherSeed(
//    uint32_t base) const
//{
//    PacketCoder::Seed seed =
//        PacketCoder::generateCipherSeed(base);
//    std::reverse(seed.begin(), seed.end());
//    return seed;
//}


void SecurePacketCoder::setEncryptSeed(const Seed& seed)
{
    cipherKeyGenerator_->setEncryptCipherKey(toCipherKey(seed));
}


void SecurePacketCoder::setDecryptSeed(const Seed& seed)
{
    cipherKeyGenerator_->setDecryptCipherKey(toCipherKey(seed));
}


bool SecurePacketCoder::readHeader(AbstractPacketHeader& header,
    const base::MemoryBlock& mblock) const
{
    return component_->readHeader(header, mblock);
}


bool SecurePacketCoder::encode(base::MemoryBlock& mblock,
    AbstractPacketHeader& header)
{
    SNE_ASSERT(mblock.rd_ptr() == mblock.base());

    const size_t headerSize = securePacketHeaderSize_;
    if (mblock.length() < headerSize) {
        return false;
    }

    align(mblock);
    header.bodySize_ = getBodySize(mblock);

    const security::Key cipherKey =
        cipherKeyGenerator_->getEncryptCipherKey();
    const FingerPrint fingerPrint = getFingerPrint(cipherKey);

    char* bodyBegin = mblock.base() + headerSize;
    const char* bodyEnd = mblock.wr_ptr();
    const size_t bodyLength = (bodyEnd - bodyBegin);

    const CheckSum checkSum =
        getCheckSum(bodyBegin, bodyEnd, fingerPrint);

    if (bodyLength > 0) {
        try {
            if (! cipher_->encrypt(bodyBegin, bodyEnd, bodyBegin, cipherKey)) {
                return false;
            }
        }
        catch (const std::exception& e) {
            SNE_LOG_ERROR("Failed to encrypt(%s)", e.what());
            return false;
        }
    }

    if (! component_->encode(mblock, header)) {
        return false;
    }

    const AdditionalPacketHeader additionalPacketHeader(fingerPrint, checkSum);
    additionalPacketHeader.write(mblock.base() + rawPacketHeaderSize_);

    SNE_ASSERT(mblock.rd_ptr() == mblock.base());

    return true;
}


bool SecurePacketCoder::decode(base::MemoryBlock& mblock)
{
    if (! component_->decode(mblock)) {
        return false;
    }

    AdditionalPacketHeader additionalPacketHeader;
    additionalPacketHeader.read(mblock.base() + rawPacketHeaderSize_);

    const security::Key cipherKey =
        cipherKeyGenerator_->getDecryptCipherKey();
    const FingerPrint fingerPrint = getFingerPrint(cipherKey);
    if (fingerPrint != additionalPacketHeader.fingerPrint_) {
        SNE_LOG_ERROR("Invalid FingerPrint(key:%u, %u != %u)",
            cipherKey, fingerPrint, additionalPacketHeader.fingerPrint_);
        return false;
    }

    char* bodyBegin = mblock.base() + securePacketHeaderSize_;
    const char* bodyEnd = mblock.wr_ptr();
    const size_t bodyLength = (bodyEnd - bodyBegin);

    if (bodyLength > 0) {
        try {
            if (! cipher_->decrypt(bodyBegin, bodyEnd, bodyBegin, cipherKey)) {
                return false;
            }
        }
        catch (const std::exception& e) {
            SNE_LOG_ERROR("Failed to encrypt(%s)", e.what());
            return false;
        }
    }

    const CheckSum checkSum =
        getCheckSum(bodyBegin, bodyEnd, fingerPrint);
    if (checkSum != additionalPacketHeader.checkSum_) {
        SNE_LOG_ERROR("Invalid CheckSum(key:%u, %u != %u)",
            cipherKey, checkSum, additionalPacketHeader.checkSum_);
        return false;
    }

    SNE_ASSERT(mblock.rd_ptr() == mblock.base());

    return true;
}


void SecurePacketCoder::extendCipherKeyTimeLimit()
{
    cipherKeyTimeLimit_ = std::chrono::steady_clock::now() +
        std::chrono::milliseconds(cipherKeyPeriod_);
}


size_t SecurePacketCoder::getMaxPacketBodySize() const
{
    return component_->getMaxPacketBodySize() - AdditionalPacketHeader::headerSize;
}


bool SecurePacketCoder::isCipherKeyExpired() const
{
    if (! shouldExchangeCipherSeed()) {
        return false;
    }

    return std::chrono::steady_clock::now() >= cipherKeyTimeLimit_;
}


void SecurePacketCoder::align(base::MemoryBlock& buffer)
{
    if (cipherBlockSize_ <= 0) {
        return;
    }

    const size_t bodySize = buffer.length() - securePacketHeaderSize_;
    const size_t mod = (bodySize % cipherBlockSize_);
    if (mod > 0) {
        const size_t delta = (cipherBlockSize_ - mod);
        buffer.reserve(delta);
        char* dummy = buffer.wr_ptr();
        for (size_t s = 0; s < delta; ++s) {
            *dummy++ = '\0';
        }
        buffer.wr_ptr(delta);
    }
}

}} // namespace sne { namespace sgp {
