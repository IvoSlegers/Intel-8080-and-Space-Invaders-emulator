#include "diagnostic_application.hpp"

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
        runDiagnostic("roms/TST8080.COM");
        //runDiagnostic("roms/CPUTEST.COM");
        //runDiagnostic("roms/8080PRE.COM");
        //runDiagnostic("roms/8080EXM.COM");

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
        consoleUI.notifyMemoryChanged();

        memory[0x0005] = 0xC9;

        cpu.setProgramCounter(0x100);

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

            if (previousPC == pc)
                continue;
            previousPC = pc;

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
    }

    void DiagnosticApplication::printOutput()
    {
        const CpuState& state = cpu.getState();
        byte operation = state.C;

        if (operation == 2) // Print one character (stored in E)
        {
            console.getScreenBuffer(1).write(std::string(1, static_cast<char>(state.E)));
        }
        else if (operation == 9) // Print characters at memory address stored in DE until '$' is encountered.
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

        if (event.isDirectInput && event.keyEvent.wVirtualKeyCode == VK_ESCAPE)
            running = false;

        if (event.isDirectInput && event.keyEvent.uChar.AsciiChar == 'a')
        {
            console.getDefaultScreenBuffer().setCursorPosition(0, console.getDefaultScreenBuffer().getWindowSize().height);
            console.getDefaultScreenBuffer().write("Running until halted.");
            runningAutonomously = true;
        }
            

        consoleUI.onConsoleEvent(event);
    }

} // namespace emulator
