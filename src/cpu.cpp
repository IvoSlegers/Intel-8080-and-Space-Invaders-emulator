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
                break;

            // DE
            case 0x19:
                state.CY = state.getDE() > (0xFFFF - state.getHL());
                state.setHL(state.getHL() + state.getDE());
                break;

            // HL
            case 0x29:
                state.CY = state.getHL() > (0xFFFF - state.getHL());
                state.setHL(state.getHL() + state.getHL());
                break;

            // BC
            case 0x39:
                state.CY = state.SP > (0xFFFF - state.getHL());
                state.setHL(state.getHL() + state.SP);
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

    void Cpu::executeINR(byte& value)
    {
        state.CA = (value & 0x0F) == 0x0F;
        ++value;
        setZSPFlags(value);
    }

    void Cpu::executeDCR(byte& value)
    {
        state.CA = (value & 0x0F) != 0;
        --value;
        setZSPFlags(value);
    }

} // namespace emulator
