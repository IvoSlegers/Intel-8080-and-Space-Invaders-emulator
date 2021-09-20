#include "console_ui.hpp"

#include "defines.hpp"
#include "application.hpp"
#include "cpu.hpp"
#include "memory.hpp"
#include "opcode_info.hpp"
#include "to_hex_string.hpp"

#include "consolegui\console.hpp"

namespace emulator
{
    InstructionsDisplay::InstructionsDisplay(Application& application_, Console& console_):
        application(application_), console(console_), cpu(application.cpu), memory(application.memory)
    {}

    unsigned int InstructionsDisplay::addInstructionToCache(word address)
    {
        Instruction instruction;

        instruction.address = address;
        instruction.opCode = memory.get(address);

        byte byte2 = 
            (static_cast<std::size_t>(address + 1) < memory.getTotalSize()) ? memory.get(address + 1) : 0;
        byte byte3 = 
            (static_cast<std::size_t>(address + 2) < memory.getTotalSize()) ? memory.get(address + 2) : 0;

        instruction.arguments = formatInstructionArguments(instruction.opCode, byte2, byte3);

        displayedInstructionsCache.push_back(instruction);

        return address + instructionLengths[instruction.opCode];
    }

    unsigned int InstructionsDisplay::addInstructionsToCache(word startAddress, unsigned int number)
    {
        unsigned int address = startAddress;
        for (unsigned int i = 0; i < number; ++i)
        {
            address = addInstructionToCache(address);

            if (address >= memory.getTotalSize())
                break;
        }

        return address;
    }

    void InstructionsDisplay::growCache(unsigned int number)
    {
        if (displayedInstructionsCache.empty())
            return;

        Instruction& lastInstruction = displayedInstructionsCache.back();

        unsigned int address = lastInstruction.address + instructionLengths[lastInstruction.opCode];

        if (address < memory.getTotalSize())
        {
            addInstructionsToCache(address, number);
        }
    }

    void InstructionsDisplay::draw(short x, short y)
    {
        std::size_t cacheSize = displayedInstructionsCache.size();
        if (firstDisplayedInstructionIndex +  numberOfInstructionsDisplayed > cacheSize)
        {
            growCache(numberOfInstructionsDisplayed + firstDisplayedInstructionIndex - cacheSize);
        }

        for (unsigned int i = 0; i < numberOfInstructionsDisplayed; ++i)
        {
            unsigned int index = firstDisplayedInstructionIndex + i;
            if (index >= cacheSize)
                break;

            const Instruction& instruction = displayedInstructionsCache[index];

            bool isSelected = (index == selectedInstructionIndex);
            bool isCurrent = (instruction.address == cpu.getState().PC);
            bool isBreakpoint = application.breakpoints.count(instruction.address) != 0;

            drawInstruction(instruction, isSelected, isCurrent, isBreakpoint, x, y + i);
        }
    } 

    void InstructionsDisplay::moveTo(word address, bool addToHistory)
    {
        displayedInstructionsCache.clear();
        addInstructionsToCache(address, numberOfInstructionsDisplayed);

        firstDisplayedInstructionIndex = 0;
        selectedInstructionIndex = 0;

        if (addToHistory)
            jumpHistory.push_back(address);
    }

    bool InstructionsDisplay::moveToTarget(const Instruction& instruction)
    {
        byte opCode = instruction.opCode;

        static const std::set<byte> controlFlowOpCodes = 
            {
                0xC2, 0xD2, 0xE2, 0xF2,
                0xC3,
                0xC4, 0xD4, 0xE4, 0xF4,
                0xCA, 0xDA, 0xEA, 0xFA,
                0xCC, 0xDC, 0xEC, 0xFC,
                0xCD,

                #if !EMULATOR_CHECK_INVALID_OPCODES
                    0xCB,
                    0xDD, 0xED, 0xFD
                #endif
            };

        // (Conditional) Call and Jump instructions
        if (controlFlowOpCodes.count(opCode) != 0)
        {
            moveTo(memory.getWord(instruction.address + 1));
            return true;
        }

        // RST instructions.
        // The opcode for RST looks like 11NNN111
        if ((opCode & 0b11000111) == 0b11000111)
        {
            moveTo(opCode && 0b001111000);
            return true;
        }

        return false;
    }

    void InstructionsDisplay::moveBack()
    {
        word address = 0;
        if (!jumpHistory.empty())
        {
            address = jumpHistory.back();
            jumpHistory.pop_back();
        }

        moveTo(address, false);
    }

    void InstructionsDisplay::assureInstructionIsDisplayed(word address)
    {
        int firstAddress = displayedInstructionsCache.front().address;
        int lastAddress = displayedInstructionsCache.back().address;

        if (address < firstAddress - 6 || address > lastAddress + 6)
        {
            moveTo(address);
            return;
        }

        if (address <= firstAddress + 6)
        {
            scrollUp(3);
            return;
        }

        if (address >= lastAddress - 6)
        {
            scrollDown(3);
            return;
        }
    }

    void InstructionsDisplay::moveSelectionUp()
    {
        if (selectedInstructionIndex > firstDisplayedInstructionIndex)
            selectedInstructionIndex--;
        else
        {
            scrollUp();

            if (selectedInstructionIndex > firstDisplayedInstructionIndex)
                selectedInstructionIndex--;
        }
    }

    void InstructionsDisplay::moveSelectionDown()
    {
        if (selectedInstructionIndex < (firstDisplayedInstructionIndex + numberOfInstructionsDisplayed - 1))
            selectedInstructionIndex++;
        else
        {
            scrollDown();

            if (selectedInstructionIndex < (firstDisplayedInstructionIndex + numberOfInstructionsDisplayed - 1))
                selectedInstructionIndex++;
        }
    }

    void InstructionsDisplay::scrollUp(unsigned short lines)
    {
        firstDisplayedInstructionIndex = std::max(
            0, static_cast<int>(firstDisplayedInstructionIndex) - lines);
    }

    void InstructionsDisplay::scrollDown(unsigned short lines)
    {
        if (firstDisplayedInstructionIndex + numberOfInstructionsDisplayed + lines >= displayedInstructionsCache.size())
        {
            growCache(lines);
        }

        firstDisplayedInstructionIndex = std::min<unsigned int>(
            displayedInstructionsCache.size() - 1,
            firstDisplayedInstructionIndex + lines);
    }

    bool InstructionsDisplay::getSelectedInstruction(Instruction& instruction)
    {
        if (selectedInstructionIndex >= displayedInstructionsCache.size())
            return false;

        instruction = displayedInstructionsCache[selectedInstructionIndex];
        return true;
    }

    void InstructionsDisplay::drawInstruction(const Instruction& instruction, bool isSelected, bool isCurrent,
        bool isBreakpoint, short x, short y)
    {
        Color foregroundColor = isSelected ? Color::Black : Color::White;
        Color backgroundColor = isSelected ? Color::White : Color::Black;

        console.setTextColor(foregroundColor, backgroundColor);

        if (isSelected)
        {
            console.setCursorPosition(x, y);
            console.write("                                                ");
        }

        if (isBreakpoint)
        {
            console.setCursorPosition(x, y);
            console.write("b");
        }

        if (isCurrent)
        {
            console.setCursorPosition(x+1, y);
            console.write(">");
        }

        console.setCursorPosition(x + 3, y);
        console.write(toHexString(instruction.address));

        console.setTextColor(Color::Blue, backgroundColor);

        console.setCursorPosition(x + 9, y);
        console.write(nmemonics[instruction.opCode]);

        console.setTextColor(foregroundColor, backgroundColor);

        console.setCursorPosition(x + 17, y);
        console.write(instruction.arguments);

        console.setTextColor(Color::White, Color::Black);
    }

    ConsoleUI::ConsoleUI(Application& application_, Console& console_):
        application(application_), console(console_), cpu(application.cpu), memory(application.memory),
        instructionsDisplay(application_, console_)
    {}

    void ConsoleUI::initialise()
    {
        instructionsDisplay.moveTo(0, false);
    }

    void ConsoleUI::draw()
    {
        console.clear();

        drawCpuState();
        drawHelp();
        drawDialog();

        console.setCursorPosition(0, console.getScreenSize().height);
    }

    void ConsoleUI::onConsoleKeyEvent(const KEY_EVENT_RECORD& event)
    {
        if (isInDialogMode)
        {
            if (event.bKeyDown)
            {
                if (event.wVirtualKeyCode == VK_RETURN)
                {
                    endDialog();

                    std::string input = dialogInput.str();
                    std::transform(input.begin(), input.end(), input.begin(),
                        [] (const char c)
                        {
                            return std::tolower(c);
                        });

                    std::string::size_type index = input.find("breakpoint");
                    if (index != std::string::npos)
                    { 
                        int address = -1;
                        try
                        {
                            address = std::stoi(input.substr(index + 11, 4), nullptr, 16);
                        }
                        catch (const std::invalid_argument& exception)
                        {
                            return;
                        };         
                        
                        if (address < 0 || static_cast<unsigned int>(address) >= memory.getTotalSize())
                        {
                            showMessage("Given address invalid.");
                        }
                        else
                        {
                            application.toggleBreakpoint(address);
                            draw();
                        }
                    }
                }
                else
                {
                    char input = event.uChar.AsciiChar;
                    dialogInput << input;
                    console.write(std::string(1, input));
                }                    
            }            
        }
        else if (event.bKeyDown)
        {
            InstructionsDisplay::Instruction instruction;
            switch (event.wVirtualKeyCode)
            {
                case 'Q':
                    application.quit();
                    break;

                case 'R':
                    application.reset();

                    if (isInFollowMode)
                    {
                        instructionsDisplay.assureInstructionIsDisplayed(cpu.getState().PC);
                    }

                    draw();
                    break;

                case VK_SPACE:
                    application.executeSingleStep();

                    if (isInFollowMode)
                    {
                        instructionsDisplay.assureInstructionIsDisplayed(cpu.getState().PC);
                    }

                    draw();
                    break;

                case 'C':
                    beginDialog("Enter command:");
                    break;

                case 'B':    
                    if (instructionsDisplay.getSelectedInstruction(instruction))
                    {
                        application.toggleBreakpoint(instruction.address);
                    }
                    draw();
                    break;

                case 'F':
                    isInFollowMode = !isInFollowMode;
                    draw();
                    break;

                case VK_UP:
                    instructionsDisplay.moveSelectionUp();
                    draw();
                    break;

                case VK_DOWN:
                    instructionsDisplay.moveSelectionDown();
                    draw();
                    break;

                case VK_RIGHT:
                    if (instructionsDisplay.getSelectedInstruction(instruction))
                    {
                        if (!instructionsDisplay.moveToTarget(instruction))
                            showMessage("Invalid instruction selected.");
                        else
                            draw();
                    }
                    break;

                case VK_LEFT:
                        instructionsDisplay.moveBack();
                        draw();
                    break;

                case VK_PRIOR: //Page Up
                    instructionsDisplay.scrollUp();
                    draw();
                    break;

                case VK_NEXT: //Page Down
                    instructionsDisplay.scrollDown();
                    draw();
                    break;
            }
        }
    }

    void ConsoleUI::drawCpuState()
    {
        const CpuState& state = cpu.getState();

        short x = 0, y = 0;
        drawRegister("A", state.A, x, y);

        x += 4;
        drawFlag("Z", state.Z, x, y);
        x += 3;
        drawFlag("S", state.S, x, y);
        x += 3;
        drawFlag("P", state.P, x, y);
        x += 3;
        drawFlag("CY", state.CY, x, y);
        x += 3;
        drawFlag("CA", state.CA, x, y);
        
        x = 0;
        y = 2;

        drawRegister("B", state.B, x, y);
        x += 2;
        drawRegister("C", state.C, x, y);

        x += 4;
        drawRegister("D", state.D, x, y);
        x += 2;
        drawRegister("E", state.E, x, y);

        x += 4;
        drawRegister("H", state.H, x, y);
        x += 2;
        drawRegister("L", state.L, x, y);

        x += 4;
        drawWordRegister("PC", state.PC, x, y);

        x += 6;
        drawWordRegister("SP", state.SP, x, y);

        console.setCursorPosition(40, 0);
        console.write("Executed instructions: " + std::to_string(cpu.getExecutedInstructionCyles()));

        console.setCursorPosition(40, 1);
        console.write("Executed cycles: " + std::to_string(cpu.getExecutedMachineCyles()));

        console.setCursorPosition(40, 2);
        console.write(std::string("Interruptions ") + (state.interruptsEnabled ? "enabled" : "disabled"));

        instructionsDisplay.draw(0, 6);
    }

    void ConsoleUI::drawRegister(const std::string& name, byte value, short x, short y)
    {
        console.setCursorPosition(x, y);
        console.write(name);
        console.setCursorPosition(x, y + 1);
        console.write(toHexString(value));
    }

    void ConsoleUI::drawWordRegister(const std::string& name, word value, short x, short y)
    {
        console.setCursorPosition(x, y);
        console.write(name);
        console.setCursorPosition(x, y + 1);
        console.write(toHexString(value));
    }

    void ConsoleUI::drawFlag(const std::string& name, bool value, short x, short y)
    {
        console.setCursorPosition(x, y);
        console.write(name);
        console.setCursorPosition(x, y + 1);
        console.write(value ? "1" : "0");
    }

    void ConsoleUI::drawHelp()
    {
        short y = 6;
        console.setCursorPosition(54, y++);
        console.write("q: Quit");

        console.setCursorPosition(54, y++);
        console.write("r: Reset");

        console.setCursorPosition(54, y++);
        console.write("space: Single step");

        console.setCursorPosition(54, y++);
        console.write("c: Enter command");

        console.setCursorPosition(54, y++);
        console.write("b: Toggle breakpoint");

        console.setCursorPosition(54, y++);
        if (isInFollowMode)
            console.write("f: Disable follow mode");
        else
            console.write("f: Enable follow mode");
    }

    void ConsoleUI::beginDialog(const std::string& prompt)
    {
        dialogInput.str(std::string());
        dialogInput.clear();

        isInDialogMode = true;
        dialogPrompt = prompt;

        draw();
    }

    void ConsoleUI::endDialog()
    {
        isInDialogMode = false;
        draw();
    }

    void ConsoleUI::drawDialog()
    {
        if (isInDialogMode)
        {
            short height = console.getScreenSize().height;

            console.setCursorPosition(0, height - 1);
            console.write(dialogPrompt);

            console.setCursorPosition(0, height);
        }
    }

    void ConsoleUI::showMessage(const std::string& message)
    {
        draw();

        console.setCursorPosition(0, console.getScreenSize().height);
        console.write(message);
    }

} // namespace emulator
