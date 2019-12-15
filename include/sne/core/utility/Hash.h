#pragma once

#include <sne/Common.h>

namespace sne { namespace core {

/// hash string
/// - PJW hash or CRC variant
/// - http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
inline uint32_t hash(const char* str, size_t len)
{
    uint32_t h = 0;

#ifdef USE_PJW_HASH
    // PJW hash
    for (size_t i = 0; i < len; ++i) {
        const char ki = str[i];
        h = (h << 4) + (ki * 13);

        const uint32_t g = hash & 0xf0000000;
        if (g != 0) {
            h ^= (g >> 24);
            h ^= g;
        }
    }
#else
    // CRC variant
    for (size_t i = 0; i < len; ++i) {
        const char ki = str[i];
        const uint32_t highorder = h & 0xf8000000;
        h <<= 5;
        h ^= (highorder >> 27);
        h ^= ki;
    }
#endif
    return h;
}


inline uint32_t hash(const char* str)
{
    return hash(str, strlen(str));
}

}} // namespace sne { namespace core
