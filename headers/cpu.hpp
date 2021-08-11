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

            void executeINR(byte& value);
            void executeDCR(byte& value);
    };
} // namespace emulator
