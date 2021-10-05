#pragma once

#include "int_types.hpp"

namespace emulator
{
    /*
        Struct for modelling the internal state of the intel 8080 processor.
        Stores the values of the 7 byte sized registers A, B, C, D, E, H, L,
        the values of the program counter (PC) and the stack pointer (SP),
        the halted state and interrupt state,
        and finally the zero (Z), sign (S), parity (P), carry (CY) and auxiliary carry (CA) state flags.         
    */
    struct CpuState
    {
        byte A = 0, B = 0, C = 0, D = 0, E = 0, H = 0, L = 0;

        word PC = 0, SP = 0;

        byte Z : 1, S : 1, P : 1, CY : 1, CA : 1;

        bool halted = false;
        bool interruptsEnabled = false;

        explicit CpuState(): Z(0), S(0), P(0), CY(0), CA(0) {}

        // Helper functions for accessing the register pairs B and C, D and E and H and L
        // as a packed word.
        word getBC() const { return bytesAsWord(B, C); }
        word getDE() const { return bytesAsWord(D, E); }
        word getHL() const { return bytesAsWord(H, L); }

        void setBC(word value) { wordAsBytePair(value, B, C); }
        void setDE(word value) { wordAsBytePair(value, D, E); }
        void setHL(word value) { wordAsBytePair(value, H, L); }

        // Packs the values of the state flags into a byte in the same manner as the intel 8080 stores 
        // the flags internally. 
        // Used for the PUSH PSW instruction. 
        byte packFlags() const
        {
           return (S << 7) | (Z << 6) | (CA << 4) | (P << 2) | (1 << 1) | CY;
        }

        // Unpacks the values of the state flags from a byte that was packed by packFlags().
        // Used for the OP PSW instruction.
        void unpackFlags(byte value)
        {
            S = (value >> 7) & 0x01;
            Z = (value >> 6) & 0x01;
            CA = (value >> 4) & 0x01;
            P = (value >> 2) & 0x01;
            CY = value & 0x01;
        }

        // Note that according to the intel specifications the only effect of RESET
        // is PC = 0, in principle all other registers and flags are left in a indeterminate state.
        // We deviate from the specifications and reset all registers and flags, to 
        // avoid possible hard to detect bugs where an instruction is influenced by a state
        // from a different run before reset() was called.
        void reset()
        {
            A = B = C = D = E = H = L = 0;
            PC = SP = 0;
            Z = S = P = CY = CA = 0;

            halted = false;
            interruptsEnabled = false;
        }
    };
} // namespace emulator
