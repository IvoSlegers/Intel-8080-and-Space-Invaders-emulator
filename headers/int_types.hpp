#pragma once

#include <cstdint>

namespace emulator
{
    using u8 = uint8_t;
    using u16 = uint16_t;

    using i8 = int8_t;
    using i16 = int16_t;

    using byte = u8;
    using word = u16;

    struct BytePair
    {
        byte high;
        byte low;
    };

    inline word bytesAsWord(byte high, byte low)
    {
        return (static_cast<word>(high) << 8) + low;
    }

    inline void wordAsBytePair(word word, byte& high, byte& low)
    {
        low = static_cast<byte>(word & 0x00FF);
        high = static_cast<byte>((word >> 8) & 0x00FF);
    }

    inline BytePair wordAsBytePair(word word)
    {
        BytePair pair;
        wordAsBytePair(word, pair.high, pair.low);
        return pair;
    }
} // namespace emulator

