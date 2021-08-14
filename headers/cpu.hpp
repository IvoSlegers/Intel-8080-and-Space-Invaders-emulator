#pragma once

#include "int_types.hpp"

namespace emulator
{
    class Memory;
    struct CpuState;

    class Cpu
    {
        public:
            explicit Cpu(Memory&, CpuState&);

            void reset();

            void executeInstructionCycle();

        private:
            Memory& memory;
            CpuState& state;

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

            void halt();
    };
} // namespace emulator
