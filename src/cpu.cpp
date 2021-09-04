#include "cpu.hpp"

#include "memory.hpp"
#include "defines.hpp"
#include "emulator_exception.hpp"
#include "to_hex_string.hpp"
#include "io.hpp"

namespace emulator
{
    Cpu::Cpu(Memory& memory_, IO& io_): memory(memory_), io(io_)
    {}

    void Cpu::reset()
    {
        executedInstructionCycles = executedMachineCyles = 0;
        state.reset();
    }

    void Cpu::executeInstructionCycle()
    {
        byte opCode = memory.get(state.PC);

        // According to table on page 2-16 of i8080 manual the program counter is always first
        // incremented by one. Then if an instruction consists of more bytes the program counter
        // is increased further.
        ++state.PC;

        word address = 0;
        word intermediate = 0;
        switch (opCode)
        {
            // NOP
            case 0x00:

            // If we do not check for invalid op codes then the following opcodes are treated as a NOP
            #if !EMULATOR_CHECK_INVALID_OPCODES
            case 0x10:
            case 0x20:
            case 0x30:
            case 0x08:
            case 0x18:
            case 0x28:
            case 0x38:
            #endif
                executedMachineCyles += 4;
                break;

            // LXI RP, Load register pair immediate
            // Note: the cpu is little endian, so the high byte (B, D or H) is after 
            // the low byte (C, E or L) in memory.

            // BC
            case 0x01:
                state.setBC(memory.getWord(state.PC));
                state.PC += 2;
                executedMachineCyles += 10;
                break;

            // DE
            case 0x11:
                state.setDE(memory.getWord(state.PC));
                state.PC += 2;
                executedMachineCyles += 10;
                break;

            // HL
            case 0x21:
                state.setHL(memory.getWord(state.PC));
                state.PC += 2;
                executedMachineCyles += 10;
                break;

            // SP
            case 0x31:
                state.SP = memory.getWord(state.PC);
                state.PC += 2;
                executedMachineCyles += 10;
                break;

            // STAX, store accumulator indirect.
            // Store the contents of register A in memory with address stored in either BC or DE.

            // BC
            case 0x02:
                memory.set(state.getBC(), state.A);
                executedMachineCyles += 7;
                break;

            // DE
            case 0x12:
                memory.set(state.getDE(), state.A);
                executedMachineCyles += 7;
                break;

            // SHLD
            // Move content of HL into memory
            case 0x22:
                address = memory.getWord(state.PC);
                memory.setWord(address, state.getHL());
                state.PC += 2;
                executedMachineCyles += 16;
                break;

            // STA 
            // Move content of A into memory
            case 0x32:
                address = memory.getWord(state.PC);
                memory.set(address, state.A);
                state.PC += 2;
                executedMachineCyles += 13;
                break;

            // INX
            // Increment register pair by one. Note: no status flags are affected.

            // BC
            case 0x03:
                state.setBC(state.getBC() + 1);
                executedMachineCyles += 5;
                break;

            // DE
            case 0x13:
                state.setDE(state.getDE() + 1);
                executedMachineCyles += 5;
                break;

            // HL
            case 0x23:
                state.setHL(state.getHL() + 1);
                executedMachineCyles += 5;
                break;

            // SP
            case 0x33:
                ++state.SP;
                executedMachineCyles += 5;
                break;

            // DCX
            // Descrease register pair by one. Note: no status flags are affected.

            // BC
            case 0x0B:
                state.setBC(state.getBC() - 1);
                executedMachineCyles += 5;
                break;

            // DE
            case 0x1B:
                state.setDE(state.getDE() - 1);
                executedMachineCyles += 5;
                break;

            // HL
            case 0x2B:
                state.setHL(state.getHL() - 1);
                executedMachineCyles += 5;
                break;

            // SP
            case 0x3B:
                --state.SP;
                executedMachineCyles += 5;
                break;

            // INR, increase register by one
            // Note: the CY flag is not set

            // B
            case 0x04:
                executeINR(state.B);                
                break;

            // D
            case 0x14:
                executeINR(state.D);
                break;

            // H
            case 0x24:
                executeINR(state.H);
                break;

            // M
            case 0x34:
                executeINR(memory[state.getHL()]);
                executedMachineCyles += 5;
                break;

            // C
            case 0x0C:
                executeINR(state.C);
                break;

            // E
            case 0x1C:
                executeINR(state.E);
                break;

            // L
            case 0x2C:
                executeINR(state.L);
                break;

            // A
            case 0x3C:
                executeINR(state.A);
                break;

            // DCR, decrease register by 1
            // Note: the CY flag is not set

            // B
            case 0x05:
                executeDCR(state.B);
                break;

            // D
            case 0x15:
                executeDCR(state.D);
                break;

            // H
            case 0x25:
                executeDCR(state.H);
                break;

            // M
            case 0x35:
                executeDCR(memory[state.getHL()]);
                executedMachineCyles += 5;
                break;

            // C
            case 0x0D:
                executeDCR(state.C);
                break;

            // E
            case 0x1D:
                executeDCR(state.E);
                break;

            // L
            case 0x2D:
                executeDCR(state.L);
                break;

            // A
            case 0x3D:
                executeDCR(state.A);
                break;

            // MVI 
            // Move data to register or memory immediate

            // B
            case 0x06:
                state.B = memory.get(state.PC);
                executedMachineCyles += 7;
                break;

            // D
            case 0x16:
                state.D = memory.get(state.PC);
                executedMachineCyles += 7;
                break;

            // H
            case 0x26:
                state.H = memory.get(state.PC);
                executedMachineCyles += 7;
                break;

            // M
            case 0x36:
                memory.set(state.getHL(), memory.get(state.PC));
                executedMachineCyles += 10;
                break;

            // C
            case 0x0E:
                state.C = memory.get(state.PC);
                executedMachineCyles += 7;
                break;

            // E
            case 0x1E:
                state.E = memory.get(state.PC);
                executedMachineCyles += 7;
                break;

            // L
            case 0x2E:
                state.L = memory.get(state.PC);
                executedMachineCyles += 7;
                break;

            // A
            case 0x3E:
                state.A = memory.get(state.PC);
                executedMachineCyles += 7;
                break;

            // RCL
            // bitwise rotate left
            case 0x07:
                state.CY = (state.A & 0x80) >> 7;
                state.A <<= 1;
                state.A |= state.CY;
                executedMachineCyles += 4;
                break;

            // RAL
            // bitwise rotate left through carry
            case 0x17:
                intermediate = (state.A & 0x80) >> 7;
                state.A <<= 1;
                state.A |= state.CY;
                state.CY = intermediate;
                executedMachineCyles += 4;
                break;

            // DAA
            // Decimal Adjust Accumulator
            case 0x27:
                if ((state.A & 0x0F) > 9 || state.CA)
                {
                    state.A += 6;
                    state.CA = 1;
                }
                else
                    state.CA = 0;

                if ((state.A & 0xFA) > (9 << 4) || state.CY)
                {
                    state.A += (9 << 4);
                    state.CY = 1;
                }
                // Note: the carry bit is not reset in this case.
                setZSPFlags(state.A);
                executedMachineCyles += 4;
                break;

            // STC
            // Set carry to 1
            case 0x37:
                state.CY = 1;
                executedMachineCyles += 4;
                break;

            // DAD
            // Add register pair to HL

            // BC
            case 0x09:
                executeDAD(state.getBC());
                break;

            // DE
            case 0x19:
                executeDAD(state.getDE());
                executedMachineCyles += 10;
                break;

            // HL
            case 0x29:
                executeDAD(state.getHL());
                break;

            // SP
            case 0x39:
                executeDAD(state.SP);
                break;

            // LDAX
            // Load accumulator from memory
            // Address stored in register

            // BC
            case 0x0A:
                state.A = memory.get(state.getBC());
                executedMachineCyles += 7;
                break;

            // DE
            case 0x1A:
                state.A = memory.get(state.getDE());
                executedMachineCyles += 7;
                break;

            // LHLD
            // Load HL from memory

            case 0x2A:
                address = memory.getWord(state.PC);
                state.setHL(memory.getWord(address));
                state.PC += 2;
                executedMachineCyles += 16;
                break;

            // LDA
            // Load accumulator from memory
            // address stored in instruction
            
            case 0x3A:
                address = memory.getWord(state.PC);
                state.A = memory.get(address);
                state.PC += 2;
                executedMachineCyles += 13;
                break;

            // RRC
            // Rotate right

            case 0x0F:
                state.CY = state.A & 0x01;
                state.A >>= 1;
                state.A |= (state.CY << 7);
                executedMachineCyles += 4;
                break;

            // RAR
            // Rotate right through carry

            case 0x1F:
                intermediate = state.A & 0x01;
                state.A >>= 1;
                state.A |= (state.CY << 7);
                state.CY = intermediate;
                executedMachineCyles += 4;
                break;

            // CMA
            // Take bitwise complement of A register

            case 0x2F:
                state.A = ~state.A;
                executedMachineCyles += 4;
                break;

            // CMC
            // The carry flag is inverted

            case 0x3F:
                state.CY = !state.CY;
                executedMachineCyles += 4;
                break;

            // MOV
            // Move register/memory to register/memory

            case 0x40: // B to B
                executedMachineCyles += 5;
                break;

            case 0x50: // B to D
                state.D = state.B;
                executedMachineCyles += 5;
                break;

            case 0x60: // B to H
                state.H = state.B;
                executedMachineCyles += 5;
                break;

            case 0x70: // B to M
                memory.set(state.getHL(), state.B);
                executedMachineCyles += 7;
                break;

            case 0x41: // C to B
                state.B = state.C;
                executedMachineCyles += 5;
                break;

            case 0x51: // C to D
                state.D = state.C;
                executedMachineCyles += 5;
                break;

            case 0x61: // C to H
                state.H = state.C;
                executedMachineCyles += 5;
                break;

            case 0x71: // C to M
                memory.set(state.getHL(), state.C);
                executedMachineCyles += 7;
                break;

            case 0x42: // D to B
                state.B = state.D;
                executedMachineCyles += 5;
                break;

            case 0x52: // D to D
                executedMachineCyles += 5;
                break;

            case 0x62: // D to H
                state.H = state.D;
                executedMachineCyles += 5;
                break;

            case 0x72: // D to M
                memory.set(state.getHL(), state.D);
                executedMachineCyles += 7;
                break;

            case 0x43: // E to B
                state.B = state.E;
                executedMachineCyles += 5;
                break;

            case 0x53: // E to D
                state.D = state.E;
                executedMachineCyles += 5;
                break;

            case 0x63: // E to H
                state.H = state.E;
                executedMachineCyles += 5;
                break;

            case 0x73: // E to M
                memory.set(state.getHL(), state.E);
                executedMachineCyles += 7;
                break;

            case 0x44: // H to B
                state.B = state.H;
                executedMachineCyles += 5;
                break;

            case 0x54: // H to D
                state.D = state.H;
                executedMachineCyles += 5;
                break;

            case 0x64: // H to H
                executedMachineCyles += 5;
                break;

            case 0x74: // H to M
                memory.set(state.getHL(), state.H);
                executedMachineCyles += 7;
                break;

            case 0x45: // L to B
                state.B = state.L;
                executedMachineCyles += 5;
                break;

            case 0x55: // L to D
                state.D = state.L;
                executedMachineCyles += 5;
                break;

            case 0x65: // L to H
                state.H = state.L;
                executedMachineCyles += 5;
                break;

            case 0x75: // L to M
                memory.set(state.getHL(), state.L);
                executedMachineCyles += 7;
                break;

            case 0x46: // M to B
                state.B = memory.get(state.getHL());
                executedMachineCyles += 5;
                break;

            case 0x56: // M to D
                state.D = memory.get(state.getHL());
                executedMachineCyles += 5;
                break;

            case 0x66: // M to H
                state.H = memory.get(state.getHL());
                executedMachineCyles += 5;
                break;

            case 0x76: // M to M
                executedMachineCyles += 7;
                halt();
                break;

            case 0x47: // A to B
                state.B = state.A;
                executedMachineCyles += 5;
                break;

            case 0x57: // A to D
                state.D = state.A;
                executedMachineCyles += 5;
                break;

            case 0x67: // A to H
                state.H = state.A;
                executedMachineCyles += 5;
                break;

            case 0x77: // A to M
                memory.set(state.getHL(), state.A);
                executedMachineCyles += 7;
                break;

            case 0x48: // B to C
                state.C = state.B;
                executedMachineCyles += 5;
                break;

            case 0x58: // B to E
                state.E = state.B;
                executedMachineCyles += 5;
                break;

            case 0x68: // B to L
                state.L = state.B;
                executedMachineCyles += 5;
                break;

            case 0x78: // B to A
                state.A = state.B;
                executedMachineCyles += 5;
                break;

            case 0x49: // C to C
                executedMachineCyles += 5;
                break;

            case 0x59: // C to E
                state.E = state.C;
                executedMachineCyles += 5;
                break;

            case 0x69: // C to L
                state.L = state.C;
                executedMachineCyles += 5;
                break;

            case 0x79: // C to A
                state.A = state.C;
                executedMachineCyles += 5;
                break;

            case 0x4A: // D to C
                state.C = state.D;
                executedMachineCyles += 5;
                break;

            case 0x5A: // D to E
                state.E = state.D;
                executedMachineCyles += 5;
                break;

            case 0x6A: // D to L
                state.L = state.D;
                executedMachineCyles += 5;
                break;

            case 0x7A: // D to A
                state.A = state.D;
                executedMachineCyles += 5;
                break;

            case 0x4B: // E to C
                state.C = state.E;
                executedMachineCyles += 5;
                break;

            case 0x5B: // E to E
                executedMachineCyles += 5;
                break;

            case 0x6B: // E to L
                state.L = state.E;
                executedMachineCyles += 5;
                break;

            case 0x7B: // E to A
                state.A = state.E;
                executedMachineCyles += 5;
                break;

            case 0x4C: // H to C
                state.C = state.H;
                executedMachineCyles += 5;
                break;

            case 0x5C: // H to E
                state.E = state.H;
                executedMachineCyles += 5;
                break;

            case 0x6C: // H to L
                state.L = state.H;
                executedMachineCyles += 5;
                break;

            case 0x7C: // H to A
                state.A = state.H;
                executedMachineCyles += 5;
                break;

            case 0x4D: // L to C
                state.C = state.L;
                executedMachineCyles += 5;
                break;

            case 0x5D: // L to E
                state.E = state.L;
                executedMachineCyles += 5;
                break;

            case 0x6D: // L to L
                executedMachineCyles += 5;
                break;

            case 0x7D: // L to A
                state.A = state.L;
                executedMachineCyles += 5;
                break;

            case 0x4E: // M to C
                state.C = memory.get(state.getHL());
                executedMachineCyles += 5;
                break;

            case 0x5E: // M to E
                state.E = memory.get(state.getHL());
                executedMachineCyles += 5;
                break;

            case 0x6E: // M to L
                state.L = memory.get(state.getHL());
                executedMachineCyles += 5;
                break;

            case 0x7E: // M to A
                state.A = memory.get(state.getHL());
                executedMachineCyles += 7;
                break;

            case 0x4F: // A to C
                state.C = state.A;
                executedMachineCyles += 5;
                break;

            case 0x5F: // A to E
                state.E = state.A;
                executedMachineCyles += 5;
                break;

            case 0x6F: // A to L
                state.L = state.A;
                executedMachineCyles += 5;
                break;

            case 0x7F: // A to A
                executedMachineCyles += 7;
                break;

            // ADD
            // Add value of specified register or memory to accumulator

            case 0x80:
                executeADD(state.B);
                break;

            case 0x81:
                executeADD(state.C);
                break;

            case 0x82:
                executeADD(state.D);
                break;

            case 0x83:
                executeADD(state.E);
                break;

            case 0x84:
                executeADD(state.H);
                break;

            case 0x85:
                executeADD(state.L);
                break;

            case 0x86:
                executeADD(memory.get(state.getHL()));
                executedMachineCyles += 3;
                break;

            case 0x87:
                executeADD(state.A);
                break;

            // SUB
            // Subtract value of specified register or memory from accumulator

            case 0x90:
                executeSUB(state.B);
                break;

            case 0x91:
                executeSUB(state.C);
                break;

            case 0x92:
                executeSUB(state.D);
                break;

            case 0x93:
                executeSUB(state.E);
                break;

            case 0x94:
                executeSUB(state.H);
                break;

            case 0x95:
                executeSUB(state.L);
                break;

            case 0x96:
                executeSUB(memory.get(state.getHL()));
                executedMachineCyles += 3;
                break;

            case 0x97:
                executeSUB(state.A);
                break;

            // ANA
            // Do a bitwise logical AND on the value of the accumulator and the specified register or memory.

            case 0xA0:
                executeANA(state.B);
                break;

            case 0xA1:
                executeANA(state.C);
                break;

            case 0xA2:
                executeANA(state.D);
                break;

            case 0xA3:
                executeANA(state.E);
                break;

            case 0xA4:
                executeANA(state.H);
                break;

            case 0xA5:
                executeANA(state.L);
                break;

            case 0xA6:
                executeANA(memory.get(state.getHL()));
                executedMachineCyles += 3;
                break;

            case 0xA7:
                executeANA(state.A);
                break;

            // ORA
            // Do a bitwise logical or on the value of the accumulator and specified register or memory.

            case 0xB0:
                executeORA(state.B);
                break;

            case 0xB1:
                executeORA(state.C);
                break;

            case 0xB2:
                executeORA(state.D);
                break;

            case 0xB3:
                executeORA(state.E);
                break;

            case 0xB4:
                executeORA(state.H);
                break;

            case 0xB5:
                executeORA(state.L);
                break;

            case 0xB6:
                executeORA(memory.get(state.getHL()));
                executedMachineCyles += 3;
                break;

            case 0xB7:
                executeORA(state.A);
                break;

            // ADC (Add with carry)
            // Add value of specified register or memory plus the carry bit to the contents of the accumulator

            case 0x88:
                executeADD(state.B, state.CY);
                break;

            case 0x89:
                executeADD(state.C, state.CY);
                break;

            case 0x8A:
                executeADD(state.D, state.CY);
                break;

            case 0x8B:
                executeADD(state.E, state.CY);
                break;

            case 0x8C:
                executeADD(state.H, state.CY);
                break;

            case 0x8D:
                executeADD(state.L, state.CY);
                break;

            case 0x8E:
                executeADD(memory.get(state.getHL()), state.CY);
                executedMachineCyles += 3;
                break;

            case 0x8F:
                executeADD(state.A, state.CY);
                break;

            // SBC (Subtract with carry)
            // Subtract value of specified register or memory plus the carry bit from the contents of the accumulator

            case 0x98:
                executeSUB(state.B, state.CY);
                break;

            case 0x99:
                executeSUB(state.C, state.CY);
                break;

            case 0x9A:
                executeSUB(state.D, state.CY);
                break;

            case 0x9B:
                executeSUB(state.E, state.CY);
                break;

            case 0x9C:
                executeSUB(state.H, state.CY);
                break;

            case 0x9D:
                executeSUB(state.L, state.CY);
                break;

            case 0x9E:
                executeSUB(memory.get(state.getHL()), state.CY);
                executedMachineCyles += 3;
                break;

            case 0x9F:
                executeSUB(state.A, state.CY);
                break;

            // XRA
            // Perform a bitwise logical or with the value of the specified register or memory and the contents of the accumulator.

            case 0xA8:
                executeXRA(state.B);
                break;

            case 0xA9:
                executeXRA(state.C);
                break;

            case 0xAA:
                executeXRA(state.D);
                break;

            case 0xAB:
                executeXRA(state.E);
                break;

            case 0xAC:
                executeXRA(state.H);
                break;

            case 0xAD:
                executeXRA(state.L);
                break;

            case 0xAE:
                executeXRA(memory.get(state.getHL()));
                executedMachineCyles += 3;
                break;

            case 0xAF:
                executeXRA(state.A);
                break;

            // CMP
            // Compare value of specified register or memory with the value of the accumulator

            case 0xB8:
                executeCMP(state.B);
                break;

            case 0xB9:
                executeCMP(state.C);
                break;

            case 0xBA:
                executeCMP(state.D);
                break;

            case 0xBB:
                executeCMP(state.E);
                break;

            case 0xBC:
                executeCMP(state.H);
                break;

            case 0xBD:
                executeCMP(state.L);
                break;

            case 0xBE:
                executeCMP(memory.get(state.getHL()));
                executedMachineCyles += 3;
                break;

            case 0xBF:
                executeCMP(state.A);
                break;

            // RNZ
            // Return when zero flag is not set
            case 0xC0:
                executeConditionalRET(!state.Z);
                break;                

            // RNC
            // Return if the carry flag is not set
            case 0xD0:
                executeConditionalRET(!state.CY);
                break;  

            // RPO
            // Return if the parity flag is set to odd (=0)
            case 0xE0:
                executeConditionalRET(!state.P);
                break;

            // RP
            // Return if sign flag is not set (plus)
            case 0xF0:
                executeConditionalRET(!state.S);
                break;

            // RZ 
            // Return if zero flag is set
            case 0xC8:
                executeConditionalRET(state.Z);
                break;

            // RC
            // Return if carry flag is set
            case 0xD8:
                executeConditionalRET(state.CY);
                break;

            // RPE
            // Return is parity flag is even (= 1)
            case 0xE8:
                executeConditionalRET(state.P);
                break;

            // RM
            // Return is sign flag is set (minus)
            case 0xF8:
                executeConditionalRET(state.S);
                break;

            // POP
            // Load register from stack

            // BC
            case 0xC1:
                state.setBC(memory.getWord(state.SP));
                state.SP += 2;
                executedMachineCyles += 10;
                break;

            // DE
            case 0xD1:
                state.setDE(memory.getWord(state.SP));
                state.SP += 2;
                executedMachineCyles += 10;
                break;

            // HL
            case 0xE1:
                state.setHL(memory.getWord(state.SP));
                state.SP += 2;
                executedMachineCyles += 10;
                break;

            // POP PSW
            // Load accumulator and flag status from stack
            case 0xF1:
                state.unpackFlags(memory.get(state.SP));
                state.A = memory.get(state.SP + 1);
                state.SP += 2;
                executedMachineCyles += 10;
                break;

            // JNZ
            // Jump if zero flag is not set
            case 0xC2:
                executeConditionalJMP(!state.Z);
                break;           

            // JNC
            // Jump if carry flag is not set
            case 0xD2:
                executeConditionalJMP(!state.CY);
                break;

            // JPO
            // Jump is parity flag is set to odd (= 0)
            case 0xE2:
                executeConditionalJMP(!state.P);
                break;

            // JP
            // Jump if sign flag is not set (positive)
            case 0xF2:
                executeConditionalJMP(!state.S);
                break;

            // JZ 
            // Jump if zero flag is set
            case 0xCA:
                executeConditionalJMP(state.Z);
                break;

            // JC
            // Jump if carry flag is set
            case 0xDA:
                executeConditionalJMP(state.CY);
                break;

            // JPE
            // Jump is parity flag is even (= 1)
            case 0xEA:
                executeConditionalJMP(state.P);
                break;

            // JM
            // Jump is sign flag is set (minus)
            case 0xFA:
                executeConditionalJMP(state.S);
                break;

            // JMP
            // Jump to memory address specified by intruction code
            case 0xC3:

            #if !EMULATOR_CHECK_INVALID_OPCODES
                case 0xCB:
            #endif

                executeConditionalJMP(true);
                break;

            // OUT
            // Put data on the data bus            
            case 0xD3:
                io.set(memory.get(state.PC), state.A);
                state.PC += 1;
                executedMachineCyles += 10;
                break;

            // XTHL
            // Exchange register HL with top of the stack
            case 0xE3:
                std::swap(state.L, memory[state.SP]);
                std::swap(state.H, memory[state.SP + 1]);
                executedMachineCyles += 18;
                break;

            // DI
            // Disable interrupts
            case 0xF3:
                setEnableInterrupts(false);
                executedMachineCyles += 4;
                break;

            // CNZ
            // Call if zero flag is not set
            case 0xC4:
                executeConditionalCALL(!state.Z);
                break;           

            // CNC
            // Call if carry flag is not set
            case 0xD4:
                executeConditionalCALL(!state.CY);
                break;

            // CPO
            // Call is parity flag is set to odd (= 0)
            case 0xE4:
                executeConditionalCALL(!state.P);
                break;

            // CP
            // Call if sign flag is not set (positive)
            case 0xF4:
                executeConditionalCALL(!state.S);
                break;

            // CZ 
            // Call if zero flag is set
            case 0xCC:
                executeConditionalCALL(state.Z);
                break;

            // CC
            // Call if carry flag is set
            case 0xDC:
                executeConditionalCALL(state.CY);
                break;

            // CPE
            // Call is parity flag is even (= 1)
            case 0xEC:
                executeConditionalCALL(state.P);
                break;

            // CM
            // Call is sign flag is set (minus)
            case 0xFC:
                executeConditionalCALL(state.S);
                break;

            // PUSH
            // Load register onto stack

            // BC
            case 0xC5:
                memory.setWord(state.SP - 2, state.getBC());
                state.SP -= 2;
                executedMachineCyles += 11;
                break;

            // DE
            case 0xD5:
                memory.setWord(state.SP - 2, state.getDE());
                state.SP -= 2;
                executedMachineCyles += 11;
                break;

            // HL
            case 0xE5:
                memory.setWord(state.SP - 2, state.getHL());
                state.SP -= 2;
                executedMachineCyles += 11;
                break;

            // PUSH PSW
            // Load accumulator and flag status onto stack
            case 0xF5:
                memory.set(state.SP - 1, state.A);
                memory.set(state.SP - 2, state.packFlags());
                state.SP -= 2;
                executedMachineCyles += 11;
                break;

            // ADI
            // Add to accumulator immediate (value encoded in instruction).
            case 0xC6:
                executeADD(memory.get(state.PC));
                ++state.PC;
                executedMachineCyles += 3;
                break;

            // SUI
            // Subtract from accumulator immediate (value encoded in instruction).
            case 0xD6:
                executeSUB(memory.get(state.PC));
                ++state.PC;
                executedMachineCyles += 3;
                break;

            // ANI
            // Perform bitwise AND with accumulator and immediate (value encoded in instruction).
            case 0xE6:
                executeANA(memory.get(state.PC));
                ++state.PC;
                executedMachineCyles += 3;
                break;

            // ORI
            // Perform bitwise OR with accumulator and immediate (value encoded in instruction).
            case 0xF6:
                executeORA(memory.get(state.PC));
                ++state.PC;
                executedMachineCyles += 3;
                break;

            // RST (Restart)
            // Change PC to address defined by bits 3, 4, 5 of the instruction opcode
            case 0xC7:
            case 0xD7:
            case 0xE7:
            case 0xF7:
            case 0xCF:
            case 0xDF:
            case 0xEF:
            case 0xFF:
                memory.setWord(state.SP - 2, state.PC);
                state.SP -= 2;
                state.PC = opCode & 0b00111000;
                executedMachineCyles += 11;
                break;

            case 0xC9:

            #if !EMULATOR_CHECK_INVALID_OPCODES
                case 0xD9:
            #endif

                executeRET();
                break;

            // PCHL
            // Jump to address specified by HL register
            case 0xE9:
                state.PC = state.getHL();
                executedMachineCyles += 5;
                break;

            // SPHL
            // Contents of HL register is moved in SP
            case 0xF9:
                state.SP = state.getHL();
                executedMachineCyles += 5;
                break;

            // IN
            // Get data on the data bus      
            case 0xDB:
                state.A = io.get(memory.get(state.PC));
                state.PC += 1;
                executedMachineCyles += 10;
                break;

            // XCHG
            // Exchange HL with DE
            case 0xEB:
                std::swap(state.H, state.D);
                std::swap(state.L, state.E);
                executedMachineCyles += 5;
                break;

            // EI 
            // Enable interrupts
            case 0xFB:
                setEnableInterrupts(true);
                executedMachineCyles += 4;
                break;

            case 0xCD:

            #if !EMULATOR_CHECK_INVALID_OPCODES
                case 0xDD:
                case 0xED:
                case 0xFD:
            #endif

                executeConditionalCALL(true);
                break;

            // ACI
            // Add to accumulator immediate with carry (value encoded in instruction).
            case 0xCE:
                executeADD(memory.get(state.PC), state.CY);
                ++state.PC;
                executedMachineCyles += 3;
                break;

            // SBI
            // Subtract from accumulator immediate with borrow (value encoded in instruction).
            case 0xDE:
                executeSUB(memory.get(state.PC), state.CY);
                ++state.PC;
                executedMachineCyles += 3;
                break;

            // XRI
            // Perform bitwise XOR with accumulator and immediate (value encoded in instruction).
            case 0xEE:
                executeXRA(memory.get(state.PC));
                ++state.PC;
                executedMachineCyles += 3;
                break;

            // CPI
            // Perform comparison between accumulator and immediate (value encoded in instruction).
            case 0xFE:
                executeCMP(memory.get(state.PC));
                ++state.PC;
                executedMachineCyles += 3;
                break;

            #if EMULATOR_CHECK_INVALID_OPCODES
            default:
                throw EmulatorException("Invalid opcode (0x" + toHexString(opCode) + ") encountered.");
            #endif
        }

        ++executedInstructionCycles;
    }

    void Cpu::setZSPFlags(byte result)
    {
        state.Z = (result == 0);
        state.S = ((result & 0x80) != 0);
        state.P = !__builtin_parity(result); // for the parity bit, 1 indicates even parity.
    }

    void Cpu::executeINR(byte& reg)
    {
        state.CA = (reg & 0x0F) == 0x0F;
        ++reg;
        setZSPFlags(reg);

        executedMachineCyles += 5;
    }

    void Cpu::executeDCR(byte& reg)
    {
        state.CA = (reg & 0x0F) != 0;
        --reg;
        setZSPFlags(reg);

        executedMachineCyles += 5;
    }

    void Cpu::executeDAD(word value)
    {
        state.CY = value > (0xFFFF - state.getHL());
        state.setHL(state.getHL() + value);

        executedMachineCyles += 10;
    }

    void Cpu::executeADD(byte value, byte carry)
    {
        state.CY = value > (0xFF - state.A - carry);
        // The auxiliary carry indicates whether there was an overflow in the addition of the last four bits only.
        state.CA = ((state.A & 0x0F) + (value & 0x0F) + carry) > 0x0F; 
        state.A += value + carry;
        setZSPFlags(state.A);

        executedMachineCyles += 4;
    }

    void Cpu::executeSUB(byte value, byte carry)
    {
        // According to the manual, the carry bit is handled by adding it to the value 
        // that will be subtracted first.
        value += carry;

        state.CY = value > state.A; // the carry flag is set when a borrow occurs.
        // The auxiliary carry indicates whether there was an overflow in the addition of the last four bits only.
        // The subtraction is performed using 2's complement representation of the second factor.
        state.CA = ((state.A & 0x0F) + ((~value + 1) & 0x0F)) > 0x0F;
        state.A -= value;
        setZSPFlags(state.A);

        executedMachineCyles += 4;
    }

    void Cpu::executeANA(byte value)
    {
        state.CY = 0;
        state.CA = ((state.A | value) & 0x08) >> 3;
        state.A &= value;
        setZSPFlags(state.A);

        executedMachineCyles += 4;
    }

    void Cpu::executeORA(byte value)
    {
        state.CY = state.CA = 0;
        state.A |= value;
        setZSPFlags(state.A);

        executedMachineCyles += 4;
    }

    void Cpu::executeXRA(byte value)
    {
        state.A ^= value;
        state.CY = state.CA = 0;
        setZSPFlags(state.A);

        executedMachineCyles += 4;
    }

    void Cpu::executeCMP(byte value)
    {
        // In a intel 8080 the CMP command is performed by doing a SUB into a temporary register
        // So all flags are set as if a subtraction was performed.
        state.CY = value > state.A; // the carry flag is set when a borrow occurs.

        // See executeSUB
        state.CA = ((state.A & 0x0F) + ((~value + 1) & 0x0F)) > 0x0F;

        setZSPFlags(state.A - value);

        executedMachineCyles += 4;
    }

    void Cpu::executeRET()
    {
        state.PC = memory.getWord(state.SP);
        state.SP += 2;

        executedMachineCyles += 10;
    }

    void Cpu::executeConditionalRET(bool condition)
    {
        if (condition)
        {
            executeRET();
            executedMachineCyles += 1;
        }
        else
            executedMachineCyles += 5;
    }

    void Cpu::executeConditionalJMP(bool condition)
    {
        if (condition)
            state.PC = memory.getWord(state.PC);

        executedMachineCyles += 10;
    }

    void Cpu::executeConditionalCALL(bool condition)
    {
        if (condition)
        {
            memory.setWord(state.SP - 2, state.getHL());
            state.SP -= 2;
            state.PC = memory.getWord(state.PC);

            executedMachineCyles += 17;
        }
        else
            executedMachineCyles += 11;
    }

    void Cpu::setEnableInterrupts(bool enabled)
    {
        return;
    }

    void Cpu::halt()
    {
        return;
    }

} // namespace emulator
