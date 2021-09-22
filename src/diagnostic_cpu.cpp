#include "diagnostic_cpu.hpp"

namespace emulator
{
    DiagnosticCpu::DiagnosticCpu(Memory& memory_, IO& io_): Cpu(memory_, io_)
    {}
    
    void DiagnosticCpu::executeIntructionCycle()
    {
        if (isBreakpoint(state.PC))
        {
            halt();
            return;
        }

        Cpu::executeInstructionCycle();
    }
    
    void DiagnosticCpu::addBreakpoint(word address)
    {
        breakpoints.insert(address);
    }

    void DiagnosticCpu::removeBreakpoint(word address)
    {
        breakpoints.erase(address);
    }

    void DiagnosticCpu::toggleBreakpoint(word address)
    {
        std::set<word>::iterator i = breakpoints.find(address);

        if (i == breakpoints.end())
            breakpoints.insert(address);           
        else
            breakpoints.erase(i);  
    }

    bool DiagnosticCpu::isBreakpoint(word address)
    {
        return breakpoints.count(address) != 0;
    }
} // namespace emulator
