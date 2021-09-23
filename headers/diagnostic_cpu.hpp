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

            void saveBreakpoints(const std::string& path);
            void loadBreakpoints(const std::string& path);

        private:
            std::set<word> breakpoints;
    };
} // namespace emulator
