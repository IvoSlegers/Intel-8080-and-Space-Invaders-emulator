#include "diagnostic_application.hpp"

#include <iostream>
#include <chrono>

namespace emulator
{
    DiagnosticApplication::DiagnosticApplication(): 
        memory(romSize, ramSize), io(), cpu(memory, io), consoleUI(console, cpu, memory)
    {
        console.createScreenBuffer();
        console.getScreenBuffer(1).setBufferSize({120, 9000});
    }

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

        console.getScreenBuffer(1).write("-- Starting diagnostic --\n");
        console.getScreenBuffer(1).write("Loading .ROM file: " + filename + '\n');

        memory.loadMemoryFromFile(filename, 0x100);
        consoleUI.signalMemoryChanged();

        memory[0x0005] = 0xC9;

        cpu.setProgramCounter(0x100);

        //std::chrono::time_point beginTime = std::chrono::high_resolution_clock::now();

        consoleUI.draw();

        running = true;
        word previousPC = cpu.getState().PC;
        while (running)
        {
            if (!runningAutonomously)
                handleEvents();
            else
            {
                console.getScreenBuffer(1).write(" - " + std::to_string(cpu.getState().PC) + (cpu.getState().halted ? " halted\n" : " running\n"));
                cpu.executeInstructionCycle();
            }                

            word pc = cpu.getState().PC;

            if (previousPC == pc)
                continue;
            previousPC = pc;

            if (pc == 0x0000)
            {
                console.getScreenBuffer(1).write("\nProgram terminated\n");
                runningAutonomously = false;
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
            console.getScreenBuffer(1).write(std::string(1, static_cast<char>(state.E)));
        }
        else if (operation == 9)
        {
            word address = state.getDE();
            while (address < memory.getTotalSize())
            {
                char character = static_cast<char>(memory.get(address++));

                if (character == '$') break;

                console.getScreenBuffer(1).write(std::string(1, static_cast<char>(character)));
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

        if (event.isDirectInput && event.keyEvent.uChar.AsciiChar == 'a')
        {
            console.getDefaultScreenBuffer().setCursorPosition(0, console.getDefaultScreenBuffer().getScreenSize().height);
            console.getDefaultScreenBuffer().write("running autonomously");
            runningAutonomously = true;
        }
            

        consoleUI.onConsoleEvent(event);
    }

} // namespace emulator
