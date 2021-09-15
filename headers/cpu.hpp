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

            void reset();

            // Returns the number of machine cycles needed to execute the command.
            std::size_t executeInstructionCycle();

            const Memory& getMemory() const { return memory;}
            const CpuState& getState() const { return state;}

            const std::size_t getExecutedInstructionCyles() const { return executedInstructionCycles; }
            const std::size_t getExecutedMachineCyles() const { return executedMachineCyles; }

            // Returns the number of machine cycles needed to execute the command.
            std::size_t issueRSTInterrupt(byte address);

            std::size_t issueRSTInterrupt(RestartInstructions instruction)
            {
                return issueRSTInterrupt(static_cast<byte>(instruction));
            }

        private:
            Memory& memory;
            IO& io;
            CpuState state;

            std::size_t executedInstructionCycles = 0;
            std::size_t executedMachineCyles = 0;

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

            void executeRET();
            void executeConditionalRET(bool condition);
            void executeConditionalJMP(bool condition);
            void executeConditionalCALL(bool condition);

            void executeRST(byte address);
            void setEnableInterrupts(bool enabled);
            void halt();
    };
} // namespace emulator
