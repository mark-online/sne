#pragma once

#include "BlockCipher.h"
#include <sne/Common.h>
#include <memory>

namespace sne { namespace security {

    /**
    * @struct BlockCipherFactory
    *
    * Block Symmetric cipher Factory
    * - supported algorithm: "aes", "tea"
    */
    struct SNE_API BlockCipherFactory
    {
        static bool isValidCipher(const std::string& cipher);

        /**
         * @param cipher algorithm
         */
        static std::unique_ptr<BlockCipher> create(const std::string& cipher);
    };

}} // namespace sne { namespace security {
