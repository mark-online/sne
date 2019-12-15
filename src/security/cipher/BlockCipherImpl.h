#pragma once

#include <sne/security/cipher/BlockCipher.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace security {

    /**
    * @class BlockCipherAlgorithm
    *
    * Block Symmetric cipher base class
    */
    template <typename Engine,
        typename InputElement = uint8_t, typename OutputElement = InputElement >
    class BlockCipherAlgorithm : public boost::noncopyable
    {
    public:
        enum {
            keyLength = Engine::DEFAULT_KEYLENGTH,
            blockSize = Engine::BLOCKSIZE
        };
    public:
        BlockCipherAlgorithm() :
              prevUpdatedKey_(0xFFFFFFFFu) {
              updateKey(0);
        }

        bool precondition(const InputElement* first,
            const InputElement* last) {
            const size_t length = (last - first);
            return (length > 0) && ((length % blockSize) == 0);
        }

        void updateKey(Key cipherKey) {
            if (cipherKey == prevUpdatedKey_) {
                return;
            }

            const size_t arrayLength = keyLength / sizeof(Key);
            Key key[arrayLength];
            for (size_t i = 0; i < arrayLength; ++i) {
                key[i] = (cipherKey >> i) || (cipherKey << i);
            }
            engine_.SetKey(
                reinterpret_cast<const InputElement*>(&key[0]), sizeof(key));

            prevUpdatedKey_ = cipherKey;
        }

        void process(const InputElement* first, const InputElement* last,
            OutputElement* output) {
            assert(precondition(first, last));
            for (;;) {
                const ptrdiff_t inputSize = last - first;
                if (inputSize <= 0) {
                    break;
                }

                engine_.ProcessBlock(first, output);

                first += blockSize;
                output += blockSize;
            }
        }

    private:
        Engine engine_;

        Key prevUpdatedKey_;
    };


    /**
    * @class BlockCipherImpl
    *
    * BlockCipherAlgorithm implementation
    */
    template <typename Encryptor, typename Decryptor>
    class BlockCipherImpl : public BlockCipher
    {
    private:
        virtual size_t getBlockSize() const override {
            assert(size_t(Encryptor::blockSize) == size_t(Decryptor::blockSize));
            return Encryptor::blockSize;
        }

        virtual bool encrypt(const void* first, const void* last, void* result,
            Key cipherKey) override {
            const uint8_t* begin = static_cast<const uint8_t*>(first);
            const uint8_t* end = static_cast<const uint8_t*>(last);

            if (! encryptor_.precondition(begin, end)) {
                return false;
            }

            encryptor_.updateKey(cipherKey);
            encryptor_.process(begin, end, static_cast<uint8_t*>(result));
            return true;
        }

        virtual bool decrypt(const void* first, const void* last, void* result,
            Key cipherKey) override {
            const uint8_t* begin = static_cast<const uint8_t*>(first);
            const uint8_t* end = static_cast<const uint8_t*>(last);

            if (! decryptor_.precondition(begin, end)) {
                return false;
            }

            decryptor_.updateKey(cipherKey);
            decryptor_.process(begin, end, static_cast<uint8_t*>(result));
            return true;
        }

    private:
        Encryptor encryptor_;
        Decryptor decryptor_;
    };

}} // namespace sne { namespace security {
