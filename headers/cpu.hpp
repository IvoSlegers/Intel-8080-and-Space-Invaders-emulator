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
            explicit Cpu(Memory&, IO&);

            void reset();

            void executeInstructionCycle();

            const Memory& getMemory() const { return memory;}
            const CpuState& getState() const { return state;}

            const std::size_t getExecutedInstructionCyles() const { return executedInstructionCycles; }
            const std::size_t getExecutedMachineCyles() const { return executedMachineCyles; }

            void issueRSTInterrupt(byte address);

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
