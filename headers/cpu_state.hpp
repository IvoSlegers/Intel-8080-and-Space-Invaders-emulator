#pragma once

#include "int_types.hpp"

namespace emulator
{
    enum class Register
    {
        A = 0b111,
        B = 0b000,
        C = 0b001,
        D = 0b010,
        E = 0b011,
        H = 0b100,
        L = 0b101
    };

    enum class RegisterPair
    {
        BC = 0b00,
        DE = 0b01,
        HL = 0b10,
        SP = 0b11,
        PC
    };

    struct CpuState
    {
        byte A = 0, B = 0, C = 0, D = 0, E = 0, H = 0, L = 0;

        word PC = 0, SP = 0;

        byte Z : 1, S : 1, P : 1, CY : 1, CA : 1;

        CpuState(): Z(0), S(0), P(0), CY(0), CA(0) {}

        word getBC() const {return bytesAsWord(B, C);}
        word getDE() const {return bytesAsWord(D, E);}
        word getHL() const {return bytesAsWord(H, L);}

        void setBC(word value) {wordAsBytePair(value, B, C);};
        void setDE(word value) {wordAsBytePair(value, D, E);};
        void setHL(word value) {wordAsBytePair(value, H, L);};

        // Note that according to the intel specifications the only effect of RESET
        // is PC = 0, all other registers and flags are left in a indeterminate state.
        void reset()
        {
            A = B = C = D = E = H = L = 0;
            PC = SP = 0;
            Z = S = P = CY = CA = 0;
        }
    };
} // namespace emulator
