#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#include <memory>

namespace sne { namespace security {

    namespace detail {
    class RsaCipherImpl;
    } // namespace detail

    /**
    * @class RsaCipher
    *
    * RSA cipher
    * - pimpl idiom
    */
    class SNE_API RsaCipher : public boost::noncopyable
    {
    public:
        enum {
            maxBufferLength = 128,
            defaultPrivateKeyLength = 512,
            defaultPrivateKeySeedLength = 64
        };

        typedef std::string Seed;

    public:
        RsaCipher();

        /// seed값을 가지고 초기화한다. 개인키와 공개키를 생성한다.
        RsaCipher(size_t privateKeyLength, const Seed& seed);

        /// 공개키, 개인키, seed값을 가지고 초기화한다.
        RsaCipher(const std::string& publicKey, const std::string& privateKey,
            const Seed& seed);

        ~RsaCipher();

        /// 생성된 공개키를 이용하여 암호화한다
        /// @return 암호화된 메시지 길이
        size_t encrypt(const void* message, size_t messageLength);

        /// 다른 공개키를 이용하여 암호화한다
        /// @return 암호화된 메시지 길이
        size_t encryptOtherPublic(const void* message, size_t messageLength,
            const std::string& otherPublicKey);

        /// 암호화된 데이터를 개인키를 이용하여 복호화한다
        /// @return 복호화된 메시지 길이
        size_t decrypt(const void* encrypted, size_t encrypted_length);

        /// 개인키를 이용하여 서명한다.
        /// @return 서명의 길이
        size_t sign(const void* message, size_t messageLength);

        /// 공개키를 이용하여 메시지의 서명이 올바른지 확인한다
        bool verify(const void* message, size_t messageLength,
            const void* signature, size_t signatureLength);

        /// 다른 공개키를 이용하여 메시지의 서명이 올바른지 확인한다
        bool verifyOtherPublic(const void* message, size_t messageLength,
            const void* signature, size_t signatureLength,
            const std::string& otherPublicKey);

        const std::string& getPublicKey() const;
        const std::string& getPrivateKey() const;

        const uint8_t* getOutputBuffer() const;

    public:
        bool encrypt(std::string& message,
            const std::string& otherPublicKey) {
            const size_t encryptedLen = encryptOtherPublic(
                message.c_str(), message.length(), otherPublicKey);
            if (encryptedLen <= 0) {
                return false;
            }
            message.assign(reinterpret_cast<const char*>(getOutputBuffer()),
                encryptedLen);
            return true;
        }

        bool decrypt(std::string& message) {
            const size_t decryptedLen = decrypt(message.c_str(), message.length());
            if (decryptedLen <= 0) {
                return false;
            }
            message.assign(reinterpret_cast<const char*>(getOutputBuffer()),
                decryptedLen);
            return true;
        }

    public:
        /// 주어진 크기(length)의 시드 값을 생성한다
        static Seed generateSeed(size_t length);

    public:
        const std::string& getLastErrorMessage() const {
            return lastErrorMessage_;
        }

    private:
        std::unique_ptr<detail::RsaCipherImpl> impl_;
        std::string lastErrorMessage_;
    };

}} // namespace sne { namespace security {
