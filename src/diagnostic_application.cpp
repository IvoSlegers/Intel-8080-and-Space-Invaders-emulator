#include "diagnostic_application.hpp"

#include <iostream>
#include <chrono>

namespace emulator
{
    DiagnosticApplication::DiagnosticApplication(): 
        memory(romSize, ramSize), io(), cpu(memory, io), consoleUI(console, cpu, memory)
    {}

    DiagnosticApplication::~DiagnosticApplication()
    {}

    void DiagnosticApplication::run()
    {
        runDiagnostic("cpu_diagnostics/TST8080.COM");
        //runDiagnostic("CPUTEST.COM");
    }

    void DiagnosticApplication::runDiagnostic(const std::string& filename)
    {
        memory.clear();
        cpu.reset();

        std::cout << "-- Starting diagnostic --\n";
        std::cout << "Loading .ROM file: " << filename << '\n';

        memory.loadMemoryFromFile(filename, 0x100);
        consoleUI.signalMemoryChanged();

        memory[0x0005] = 0xC9;

        cpu.setProgramCounter(0x100);

        //std::chrono::time_point beginTime = std::chrono::high_resolution_clock::now();

        consoleUI.draw();

        running = true;
        while (running)
        {
            handleEvents();

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
        }

        // std::chrono::time_point endTime = std::chrono::high_resolution_clock::now();
        // std::chrono::duration duration = endTime - beginTime;

        // std::chrono::minutes minutes = 
        //     std::chrono::duration_cast<std::chrono::minutes>(duration);
        // duration -= minutes;

        // std::chrono::seconds seconds = 
        //     std::chrono::duration_cast<std::chrono::seconds>(duration);
        // duration -= seconds;

        // std::chrono::milliseconds milliseconds = 
        //     std::chrono::duration_cast<std::chrono::milliseconds>(duration);

        // std::cout << "Time elapsed: " << minutes.count() << ":" << seconds.count() << "." << milliseconds.count() << '\n';
    }

    void DiagnosticApplication::printOutput()
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

    void DiagnosticApplication::handleEvents()
    {
        Console::Event event;
        if (console.waitForEvent(event))
            onConsoleEvent(event);
        
        while (console.pollEvent(event))
            onConsoleEvent(event);
    }

    void DiagnosticApplication::onConsoleEvent(const Console::Event& event)
    {
        if (event.isDirectInput && event.keyEvent.wVirtualKeyCode == VK_ESCAPE)
            running = false;

        if (event.isDirectInput && event.keyEvent.uChar.AsciiChar == 'q')
            running = false;

        consoleUI.onConsoleEvent(event);
    }

} // namespace emulator
