#include <iostream>

#include "int_types.hpp"
#include "cpu_state.hpp"

using namespace emulator;
#include <bitset>

void f(u8 value)
{
    std::cout << std::bitset<8>(value) << '\n';
}

u8 rcl(u8 value)
{
    u8 c = (value >> 7);
    value <<= 1;
    return (value | c);
}

int main()
{
    for (int i = 0; i < 256; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            uint8_t si = static_cast<uint8_t>(i);
            uint8_t sj = static_cast<uint8_t>(j);

            bool moverflowtest = si > (255 - sj);
            bool overflow = (i + j) > 255;

            if (moverflowtest != overflow)
                std::cout << "Problem at " << i << ", " << j << '\n';
        }
    }

    std::cout << "NO problems!";
    return EXIT_SUCCESS;
}