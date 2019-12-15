#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace security {

    typedef uint32_t Key;

    /**
    * @class BlockCipher
    *
    * Block Symmetric cipher base class
    */
    class BlockCipher : public boost::noncopyable
    {
    public:
        virtual ~BlockCipher() {}

        /// block의 크기를 리턴한다.
        virtual size_t getBlockSize() const = 0;

        /**
         * [first, last)를 암호화하여 result에 기록한다.
         * @pre result의 버퍼 크기는 getBlockSize()의 배수이어야 하며, 결과를
         *      저장할 수 있을만큼 충분히 커야 한다.
         */
        virtual bool encrypt(const void* first, const void* last, void* result,
            Key cipherKey) = 0;

        /**
         * [first, last)를 복호화하여 result에 기록한다.
         * @pre result의 버퍼 크기는 getBlockSize()의 배수이어야 하며, 결과를
         *      저장할 수 있을만큼 충분히 커야 한다.
         */
        virtual bool decrypt(const void* first, const void* last, void* result,
            Key cipherKey) = 0;
    };

}} // namespace sne { namespace security {
