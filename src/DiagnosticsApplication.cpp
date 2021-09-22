#include "diagnostics_application.hpp"

#include <iostream>
#include <chrono>

namespace emulator
{
    DiagnosticsApplication::DiagnosticsApplication(): 
        memory(romSize, ramSize), io(), cpu(memory, io)
    {}

    DiagnosticsApplication::~DiagnosticsApplication()
    {}

    void DiagnosticsApplication::run()
    {
        runDiagnostic("cpu_diagnostics/TST8080.COM");
        //runDiagnostic("CPUTEST.COM");

        std::cin.get();
    }

    void DiagnosticsApplication::runDiagnostic(const std::string& filename)
    {
        memory.clear();
        cpu.reset();

        std::cout << "-- Starting diagnostic --\n";
        std::cout << "Loading .ROM file: " << filename << '\n';

        memory.loadMemoryFromFile(filename, 0x100);

        memory[0x0005] = 0xC9;

        cpu.setProgramCounter(0x100);

        std::chrono::time_point beginTime = std::chrono::high_resolution_clock::now();

        while (true)
        {
            word pc = cpu.getState().PC;

            if (pc == 0x0000)
            {
                std::cout << "\nProgram terminated\n";
                break;
            }

            if (pc == 0x0005)
            {
                printOutput();
            }

            cpu.executeInstructionCycle();
        }

        std::chrono::time_point endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration duration = endTime - beginTime;

        std::chrono::minutes minutes = 
            std::chrono::duration_cast<std::chrono::minutes>(duration);
        duration -= minutes;

        std::chrono::seconds seconds = 
            std::chrono::duration_cast<std::chrono::seconds>(duration);
        duration -= seconds;

        std::chrono::milliseconds milliseconds = 
            std::chrono::duration_cast<std::chrono::milliseconds>(duration);

        std::cout << "Time elapsed: " << minutes.count() << ":" << seconds.count() << "." << milliseconds.count() << '\n';
    }

    void DiagnosticsApplication::printOutput()
    {
        const CpuState& state = cpu.getState();
        byte operation = state.C;

        if (operation == 2) // Print one character (stored in E)
        {
            std::cout << static_cast<char>(state.E);
        }
        else if (operation == 9)
        {
            word address = state.getDE();
            while (address < memory.getTotalSize())
            {
                char character = static_cast<char>(memory.get(address++));

                if (character == '$') break;

                std::cout << character;
            }   
        }        
    }

} // namespace emulator
