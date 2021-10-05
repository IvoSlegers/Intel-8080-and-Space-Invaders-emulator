#pragma once

#include <cstdint>

namespace emulator
{
    /*
        Definitions of the byte (8 bits) and word (16 bits) unsigned integer types.
    */
    using byte = uint8_t;
    using word = uint16_t;

    /*
        Helper struct to unpack a word of the form <high byte><low byte> into two bytes.
    */
    struct BytePair
    {
        byte high;
        byte low;
    };

    /*
        Functions for packing two bytes into words and unpacking words into bytes.
    */

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

