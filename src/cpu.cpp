#include "cpu.hpp"

#include "cpu_state.hpp"
#include "memory.hpp"
#include "defines.hpp"

namespace emulator
{
    Cpu::Cpu(Memory& memory_, CpuState& state_): memory(memory_), state(state_)
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
            // Move data at address stored in either BC or DE into A

            // BC
            case 0x02:
                state.A = memory.get(state.getBC());
                executedMachineCyles += 7;
                break;

            // DE
            case 0x12:
                state.A = memory.get(state.getDE());
                executedMachineCyles += 7;
                break;

            // SHLD
            // Move content of HL into memory
            case 0x22:
                address = memory.getWord(state.PC);
                memory.set(address, state.L);
                memory.set(address + 1, state.H);
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
                executedMachineCyles += 5;
                break;

            // D
            case 0x14:
                executeINR(state.D);
                executedMachineCyles += 5;
                break;

            // H
            case 0x24:
                executeINR(state.H);
                executedMachineCyles += 5;
                break;

            // M
            case 0x34:
                executeINR(memory[state.getHL()]);
                executedMachineCyles += 10;
                break;

            // C
            case 0x0C:
                executeINR(state.C);
                executedMachineCyles += 5;
                break;

            // E
            case 0x1C:
                executeINR(state.E);
                executedMachineCyles += 5;
                break;

            // L
            case 0x2C:
                executeINR(state.L);
                executedMachineCyles += 5;
                break;

            // A
            case 0x3C:
                executeINR(state.A);
                executedMachineCyles += 5;
                break;

            // DCR, decrease register by 1
            // Note: the CY flag is not set

            // B
            case 0x05:
                executeDCR(state.B);
                executedMachineCyles += 5;
                break;

            // D
            case 0x15:
                executeDCR(state.D);
                executedMachineCyles += 5;
                break;

            // H
            case 0x25:
                executeDCR(state.H);
                executedMachineCyles += 5;
                break;

            // M
            case 0x35:
                executeDCR(memory[state.getHL()]);
                executedMachineCyles += 10;
                break;

            // C
            case 0x0D:
                executeDCR(state.C);
                executedMachineCyles += 5;
                break;

            // E
            case 0x1D:
                executeDCR(state.E);
                executedMachineCyles += 5;
                break;

            // L
            case 0x2D:
                executeDCR(state.L);
                executedMachineCyles += 5;
                break;

            // A
            case 0x3D:
                executeDCR(state.A);
                executedMachineCyles += 5;
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
                state.CY = state.getBC() > (0xFFFF - state.getHL());
                state.setHL(state.getHL() + state.getBC());
                executedMachineCyles += 10;
                break;

            // DE
            case 0x19:
                state.CY = state.getDE() > (0xFFFF - state.getHL());
                state.setHL(state.getHL() + state.getDE());
                executedMachineCyles += 10;
                break;

            // HL
            case 0x29:
                state.CY = state.getHL() > (0xFFFF - state.getHL());
                state.setHL(state.getHL() + state.getHL());
                executedMachineCyles += 10;
                break;

            // SP
            case 0x39:
                state.CY = state.SP > (0xFFFF - state.getHL());
                state.setHL(state.getHL() + state.SP);
                executedMachineCyles += 10;
                break;

            // LDAX
            // Load accumulator from memory
            // Address stored in register

            // BC
            case 0x0A:
                memory.set(state.getBC(), state.A);
                executedMachineCyles += 7;
                break;

            // DE
            case 0x1A:
                memory.set(state.getDE(), state.A);
                executedMachineCyles += 7;
                break;

            //LHLD
            // Load HL from memory

            case 0x2A:
                address = memory.getWord(state.PC);
                state.L = memory.get(address);
                state.H = memory.get(address + 1);
                state.PC += 2;
                executedMachineCyles += 16;
                break;

            // LDA
            // Load accumulator from memory
            // address stored in instruction
            
            case 0x3A:
                address = memory.getWord(state.PC);
                memory.set(address, state.A);
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

            case 0x80:
                executeADD(state.B);
                executedMachineCyles += 4;
                break;

            case 0x81:
                executeADD(state.C);
                executedMachineCyles += 4;
                break;

            case 0x82:
                executeADD(state.D);
                executedMachineCyles += 4;
                break;

            case 0x83:
                executeADD(state.E);
                executedMachineCyles += 4;
                break;

            case 0x84:
                executeADD(state.H);
                executedMachineCyles += 4;
                break;

            case 0x85:
                executeADD(state.L);
                executedMachineCyles += 4;
                break;

            case 0x86:
                executeADD(memory.get(state.PC));
                executedMachineCyles += 4;
                break;

            case 0x87:
                executeADD(state.A);
                executedMachineCyles += 4;
                break;

            case 0x90:
                executeSUB(state.B);
                executedMachineCyles += 4;
                break;

            case 0x91:
                executeSUB(state.C);
                executedMachineCyles += 4;
                break;

            case 0x92:
                executeSUB(state.D);
                executedMachineCyles += 4;
                break;

            case 0x93:
                executeSUB(state.E);
                executedMachineCyles += 4;
                break;

            case 0x94:
                executeSUB(state.H);
                executedMachineCyles += 4;
                break;

            case 0x95:
                executeSUB(state.L);
                executedMachineCyles += 4;
                break;

            case 0x96:
                executeSUB(memory.get(state.PC));
                executedMachineCyles += 4;
                break;

            case 0x97:
                executeSUB(state.A);
                executedMachineCyles += 4;
                break;

            // B D H M C E L A
            // BC DE HL SP
        }

        ++executedInstructionCycles;
    }

    void Cpu::setZSPFlags(byte result)
    {
        state.Z = (result == 0);
        state.S = ((result & 0x80) != 0);
        state.P = __builtin_parity(result);
    }

    void Cpu::executeINR(byte& reg)
    {
        state.CA = (reg & 0x0F) == 0x0F;
        ++reg;
        setZSPFlags(reg);
    }

    void Cpu::executeDCR(byte& reg)
    {
        state.CA = (reg & 0x0F) != 0;
        --reg;
        setZSPFlags(reg);
    }

    void Cpu::executeADD(byte value)
    {
        state.CY = value > (0xFF - state.A);
        // The auxiliary carry indicates whether there was an overflow in the addition of the last four bits only.
        state.CA = ((state.A & 0x0F) + (value & 0x0F)) > 0x0F; 
        state.A += value;
        setZSPFlags(state.A);
    }

    void Cpu::executeSUB(byte value)
    {
        state.CY = value > state.A; // the carry flag is set when a borrow occurs.
        // The auxiliary carry indicates whether there was an overflow in the addition of the last four bits only.
        // The subtraction is performed using 2's complement representation of the second factor.
        state.CA = ((state.A & 0x0F) + ((~value + 1) & 0x0F)) > 0x0F;
        state.A -= value;
        setZSPFlags(state.A);
    }

} // namespace emulator
