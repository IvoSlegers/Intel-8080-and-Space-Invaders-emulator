#include <iostream>

#include "int_types.hpp"
#include "cpu_state.hpp"

using namespace emulator;
#include <bitset>

void f(u8 value)
{
    std::cout << std::bitset<8>(value) << '\n';
}

// TODO:
// MAKE A READ/PACK FLAGS AS BYTE FUNCTION.

void test(u8 A, u8 value)
{
    u8 CY = value > A; // the carry flag is set when a borrow occurs.
    // The auxiliary carry indicates whether there was an overflow in the addition of the last four bits only.
    // The subtraction is performed using 2's complement representation of the second factor.
    u8 CA = ((A & 0x0F) + ((~value + 1) & 0x0F)) > 0x0F;
    A -= value;
    u8 result = A;
    u8 Z = (result == 0);
    u8 S = ((result & 0x80) != 0);
    u8 P = !__builtin_parity(result); 

    std::cout << "carry " << (int)CY << '\n';
    std::cout << "sign " << (int)S << '\n';
    std::cout << "zero " << (int)Z << '\n';
    std::cout << "parity " << (int)P << '\n';
    std::cout << "aux carry " << (int)CA << '\n';
}

int main()
{
    test(0x3A, 0x3A);

    std::cout << "NO problems!";
    return EXIT_SUCCESS;
}