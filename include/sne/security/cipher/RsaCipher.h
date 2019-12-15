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

        /// seed���� ������ �ʱ�ȭ�Ѵ�. ����Ű�� ����Ű�� �����Ѵ�.
        RsaCipher(size_t privateKeyLength, const Seed& seed);

        /// ����Ű, ����Ű, seed���� ������ �ʱ�ȭ�Ѵ�.
        RsaCipher(const std::string& publicKey, const std::string& privateKey,
            const Seed& seed);

        ~RsaCipher();

        /// ������ ����Ű�� �̿��Ͽ� ��ȣȭ�Ѵ�
        /// @return ��ȣȭ�� �޽��� ����
        size_t encrypt(const void* message, size_t messageLength);

        /// �ٸ� ����Ű�� �̿��Ͽ� ��ȣȭ�Ѵ�
        /// @return ��ȣȭ�� �޽��� ����
        size_t encryptOtherPublic(const void* message, size_t messageLength,
            const std::string& otherPublicKey);

        /// ��ȣȭ�� �����͸� ����Ű�� �̿��Ͽ� ��ȣȭ�Ѵ�
        /// @return ��ȣȭ�� �޽��� ����
        size_t decrypt(const void* encrypted, size_t encrypted_length);

        /// ����Ű�� �̿��Ͽ� �����Ѵ�.
        /// @return ������ ����
        size_t sign(const void* message, size_t messageLength);

        /// ����Ű�� �̿��Ͽ� �޽����� ������ �ùٸ��� Ȯ���Ѵ�
        bool verify(const void* message, size_t messageLength,
            const void* signature, size_t signatureLength);

        /// �ٸ� ����Ű�� �̿��Ͽ� �޽����� ������ �ùٸ��� Ȯ���Ѵ�
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
        /// �־��� ũ��(length)�� �õ� ���� �����Ѵ�
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
