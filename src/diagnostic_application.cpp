#include "diagnostic_application.hpp"

#include <iostream>

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
        beginChooseTestPrompt();

        while (state != State::Finished)
        {
            handleEvents();

            if (state == State::CpuPaused)
            {
                handleTestIO();
                continue;
            }

            if (state == State::CpuRunning)
            {
                for (std::size_t i = 0; i < 10000 && state == State::CpuRunning; ++i)
                {
                    cpu.executeInstructionCycle();

                    if (cpu.getState().halted)
                    {
                        cpu.resume();
                        state = State::CpuPaused;
                        continue;
                    }

                    handleTestIO();
                }
            }
        }
    }

    void DiagnosticApplication::beginTest(const std::string& filename)
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

        state = State::CpuPaused;
    }

    void DiagnosticApplication::handleTestIO()
    {
        word pc = cpu.getState().PC;

        if (pc == previousPC)
            return;

        if (pc == 0x0000)
        {
            console.getScreenBuffer(1).write("\nProgram terminated\n\n\n");
            state = State::CpuPaused;

            consoleUI.draw();
        }            

        if (pc == 0x0005)
            printOutput();

        previousPC = pc;
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

    void DiagnosticApplication::beginChooseTestPrompt()
    {
        console.restoreDefaultScreenBuffer();
        console.clear();

        console.write("Choose test to run:\n");
        console.write("1: TST8080.COM\n");
        console.write("2: CPUTEST.COM\n");
        console.write("3: 8080PRE.COM\n");
        console.write("4: 8080EXM.COM");

        console.setCursorPosition(0, 8);
        console.write("Press 1, 2, 3 or 4 to select a test or press ESC to quit.");

        state = State::ChoosingTest;
    }

    void DiagnosticApplication::handleEvents()
    {
        Console::Event event;
        if (state != State::CpuRunning && console.waitForEvent(event))
            onConsoleEvent(event);
        
        while (console.pollEvent(event))
            onConsoleEvent(event);
    }

    void DiagnosticApplication::onConsoleEvent(const Console::Event& event)
    {
        if (!event.isDirectInput)
        {
            consoleUI.onConsoleEvent(event);
            return;
        }

        if (event.keyEvent.bKeyDown && event.keyEvent.wVirtualKeyCode == VK_ESCAPE)
        {
            if (state == State::ChoosingTest)
                state = State::Finished;
            else if (state == State::CpuRunning)
                state = State::CpuPaused;
            else if (state == State::CpuPaused)
                beginChooseTestPrompt();

            return;
        }

        if (state == State::ChoosingTest)
        {
            if (event.keyEvent.bKeyDown)
            {
                switch (event.keyEvent.wVirtualKeyCode)
                {
                    case '1':
                        beginTest("roms/TST8080.COM"); 
                        break;

                    case '2':
                        beginTest("roms/CPUTEST.COM");
                        break;

                    case '3':
                        beginTest("roms/8080PRE.COM");
                        break;

                    case '4':
                        beginTest("roms/8080EXM.COM");
                        break;
                }
            }

            return;
        }

        if (state == State::CpuPaused)
        {
            if (event.keyEvent.bKeyDown && event.keyEvent.wVirtualKeyCode == 'A')
            {
                state = State::CpuRunning;

                console.getDefaultScreenBuffer().setCursorPosition(
                    0, console.getDefaultScreenBuffer().getWindowSize().height);
                console.write("Running program until halt encounted.");
            }
            else 
                consoleUI.onConsoleEvent(event);
        }            
    }

} // namespace emulator
