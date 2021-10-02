#include "diagnostic_application.hpp"

#include <iostream>
#include <chrono>

// remove?
#include "to_hex_string.hpp"
#include "opcode_info.hpp"

namespace emulator
{
    DiagnosticApplication::DiagnosticApplication(): 
        memory(romSize, ramSize), io(), cpu(memory, io), consoleUI(console, cpu, memory)
    {
        console.createScreenBuffer();
        console.getScreenBuffer(1).setBufferSize({120, 9000});

        console.createScreenBuffer();
        console.getScreenBuffer(2).setBufferSize({120, 9000});
    }

    DiagnosticApplication::~DiagnosticApplication()
    {}

    void DiagnosticApplication::run()
    {
        runDiagnostic("cpu_diagnostics/TST8080.COM");
        runDiagnostic("cpu_diagnostics/CPUTEST.COM");
        runDiagnostic("cpu_diagnostics/8080PRE.COM");
        runDiagnostic("cpu_diagnostics/8080EXM.COM");

        runningAutonomously = false;
        running = true;
        while (running)
        {
            handleEvents();
        }
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

            if (runningAutonomously)
            {
                cpu.executeInstructionCycle();

                if (cpu.getState().halted)
                {
                    runningAutonomously = false;
                    cpu.resume();

                    consoleUI.draw();
                }        
            }

            word pc = cpu.getState().PC;

            // if (pc == 0x31b3 && cpu.getState().getHL() < 0xffff)
            // {
            //     const_cast<CpuState&>(cpu.getState()).setHL(0xffff);
            //     const_cast<CpuState&>(cpu.getState()).A = 0xf5;

            //     Console::Event event;
            //     while (console.pollEvent(event));
            //     runningAutonomously = false;
            // }

            if (previousPC == pc)
                continue;
            previousPC = pc;

            // console::ScreenBuffer& buffer = console.getScreenBuffer(2);

            // buffer.write(toHexString(pc));
            // buffer.write("  ");

            // byte opCode = memory.get(pc);

            // buffer.write(nmemonics[opCode]);

            // int i = 6 - nmemonics[opCode].size();
            // buffer.write(std::string(i, ' '));

            // byte byte2 = 
            // (static_cast<std::size_t>(pc + 1) < memory.getTotalSize()) ? memory.get(pc + 1) : 0;
            // byte byte3 = 
            //     (static_cast<std::size_t>(pc + 2) < memory.getTotalSize()) ? memory.get(pc + 2) : 0;

            // buffer.write(formatInstructionArguments(opCode, byte2, byte3));
            // buffer.write("\n");

            if (pc == 0x0000)
            {
                console.getScreenBuffer(1).write("\nProgram terminated\n");
                running = false;
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
        if (!runningAutonomously && console.waitForEvent(event))
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
