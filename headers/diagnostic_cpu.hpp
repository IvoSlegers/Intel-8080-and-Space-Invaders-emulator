#pragma once

#include "cpu.hpp"

#include <set>

namespace emulator
{
    class DiagnosticCpu : public Cpu
    {
        public:
            DiagnosticCpu(Memory&, IO&); 
            
            void executeIntructionCycle();
            
            void addBreakpoint(word address);
            void removeBreakpoint(word address);
            void toggleBreakpoint(word address);
            bool isBreakpoint(word address);

        private:
            std::set<word> breakpoints;
    };
} // namespace emulator
