#pragma once

#include "cpu.hpp"

#include <set>

namespace emulator
{
    /*
        Extention of the base cpu class which has the ability to set breakpoints.
        The cpu automatically enters the halted state whenever it encounters a breakpoint.
    */
    class DiagnosticCpu : public Cpu
    {
        public:
            DiagnosticCpu(Memory&, IO&); 
            
            // Execute the instruction currently pointed at by the program counter.
            // Returns the number of machine cycles the instruction took to execute.
            std::size_t executeInstructionCycle();
            
            void addBreakpoint(word address);
            void removeBreakpoint(word address);
            void toggleBreakpoint(word address);
            bool isBreakpoint(word address);

            // Saves or loads the set of breakpoints to a file specified by path.
            // Throws an EmulatorException is the file specified by path could not be opened.
            void saveBreakpoints(const std::string& path);
            void loadBreakpoints(const std::string& path);

        private:
            std::set<word> breakpoints;
    };
} // namespace emulator
