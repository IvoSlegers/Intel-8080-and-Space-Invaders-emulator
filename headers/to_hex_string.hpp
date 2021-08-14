#pragma once

#include "int_types.hpp"

#include <string>

namespace emulator
{
    constexpr const char* hexSymbols = "0123456789ABCDEF";

    inline std::string toHexString(u8 value)
    {
        std::string result(2, '0');
        result[0] = hexSymbols[(value >> 4) & 0x0F];
        result[1] = hexSymbols[value & 0x0F];
        return result;
    } 

    inline std::string toHexString(u16 value)
    {
        std::string result(4, '0');
        result[0] = hexSymbols[(value >> 12) & 0x0F];
        result[1] = hexSymbols[(value >> 8) & 0x0F];
        result[2] = hexSymbols[(value >> 4) & 0x0F];
        result[3] = hexSymbols[value & 0x0F];
        return result;
    }
} // namespace emulator
