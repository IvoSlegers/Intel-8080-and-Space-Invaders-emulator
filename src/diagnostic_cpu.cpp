#include "diagnostic_cpu.hpp"

#include "emulator_exception.hpp"

#include <fstream>

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

    void DiagnosticCpu::saveBreakpoints(const std::string& path)
    {
        std::ofstream file(path);

        if (!file)
            throw EmulatorException("Unable to open file " + path + '.');

        for (word address : breakpoints)
            file << address << ' ';
    }

    void DiagnosticCpu::loadBreakpoints(const std::string& path)
    {
        std::ifstream file(path);

        if (!file)
            throw EmulatorException("Unable to open file " + path + '.');

        breakpoints.clear();
        word address;
        while (file >> address)
        {
            breakpoints.insert(breakpoints.end(), address);
        }
    }
} // namespace emulator
