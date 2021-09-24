#include "diagnostic_cpu.hpp"

#include "emulator_exception.hpp"

#include <fstream>

#include <iostream>

namespace emulator
{
    DiagnosticCpu::DiagnosticCpu(Memory& memory_, IO& io_): Cpu(memory_, io_)
    {}
    
    std::size_t DiagnosticCpu::executeInstructionCycle()
    {
        std::size_t machineCycles = Cpu::executeInstructionCycle();

        // if (isBreakpoint(state.PC))
        // {
        //     halt();
        //     return 0;
        // }

        return machineCycles;
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
            throw EmulatorException("Unable to open file " + path + " in DiagnosticCpu::saveBreakpoints.");

        for (word address : breakpoints)
            file << address << ' ';
    }

    void DiagnosticCpu::loadBreakpoints(const std::string& path)
    {
        std::ifstream file(path);

        if (!file)
            throw EmulatorException("Unable to open file " + path + " in DiagnosticCpu::loadBreakpoints.");

        breakpoints.clear();
        word address;
        while (file >> address)
        {
            breakpoints.insert(breakpoints.end(), address);
        }
    }
} // namespace emulator
