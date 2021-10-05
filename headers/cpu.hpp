#pragma once

#include "int_types.hpp"
#include "cpu_state.hpp"

namespace emulator
{
    class Memory;
    class IO;

    class Cpu
    {
        public:
            /*
                Enum of the RST0 through RST7 instructions.
                Enum value is the address to which each instruction jumps.
            */
            enum class RestartInstructions : byte
            {
                RST0 = 0,
                RST1 = 8,
                RST2 = 16,
                RST3 = 24,
                RST4 = 32,
                RST5 = 40,
                RST6 = 48,
                RST7 = 56
            };
            
        public:
            explicit Cpu(Memory&, IO&);

            // Resets the cpu state and instruction and machine cycle counters.
            void reset();

            // Execute the instruction pointed at by the program counter.
            // Returns the number of machine cycles needed to execute the command.
            // If cpu is in halted state, does nothing and returns 0.
            std::size_t executeInstructionCycle();

            // Execute instructions until a halted state is reached.
            // Caution: If memory is loaded with a program that does not reach an halted state
            // then this function will hang.
            // Returns the number of machine cycles executed.
            std::size_t executeUntilHalt();

            const Memory& getMemory() const { return memory; }
            const CpuState& getState() const { return state; }

            const std::size_t getExecutedInstructionCyles() const { return executedInstructionCycles; }
            const std::size_t getExecutedMachineCyles() const { return executedMachineCycles; }

            // Emulates the the response of the intel 8080 when the INTERRUPT input is set to high
            // and a RSTn instruction is put on the data bus.
            // Caution: Does not completely accurately mimick the intel 8080.
            // Namely, the RSTn instruction is immediately executed when this method is callled.
            // Rather than waiting for the appropriate number of machine cycles.
            // Returns the number of machine cycles needed to execute the command.
            std::size_t issueRSTInterrupt(byte address);

            std::size_t issueRSTInterrupt(RestartInstructions instruction)
            {
                return issueRSTInterrupt(static_cast<byte>(instruction));
            }

            void halt() { state.halted = true; } 
            void resume() { state.halted = false; }
            void setProgramCounter(word address) { state.PC = address; }

        protected:
            Memory& memory;
            IO& io;
            CpuState state;

            std::size_t executedInstructionCycles = 0;
            std::size_t executedMachineCycles = 0;

        private:
            // Sets the zero, sign and parity flags depending on the state of register A.
            void setZSPFlags(byte result);

            void executeINR(byte& reg);
            void executeDCR(byte& reg);

            void executeDAD(word value);

            void executeADD(byte value, byte carry = 0);
            void executeSUB(byte value, byte carry = 0);
            void executeANA(byte value);
            void executeORA(byte value);
            void executeXRA(byte value);
            void executeCMP(byte value);

            void executeDAA();

            void executeRET();
            void executeConditionalRET(bool condition);
            void executeConditionalJMP(bool condition);
            void executeConditionalCALL(bool condition);

            void executeRST(byte address);
            void setEnableInterrupts(bool enabled);
    };
} // namespace emulator
