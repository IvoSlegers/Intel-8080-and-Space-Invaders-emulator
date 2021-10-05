#include "console_ui.hpp"

#include "defines.hpp"
#include "spaceinvaders_application.hpp"
#include "diagnostic_cpu.hpp"
#include "memory.hpp"
#include "opcode_info.hpp"
#include "to_hex_string.hpp"

#include "consolegui\console.hpp"
using Color = console::Color;

#include <regex>

namespace emulator
{
    InstructionsDisplay::InstructionsDisplay(Console& console_,
        DiagnosticCpu& cpu_, Memory& memory_):
        console(console_), cpu(cpu_), memory(memory_)
    {}

    void InstructionsDisplay::draw(short x, short y)
    {
        // Grow the displayed instructions cache it holds fewer items than we want to display.
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
            bool isBreakpoint = cpu.isBreakpoint(instruction.address);

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

        // List of instructions (JMP, CALL, RET) which encode a target address in the memory next to them
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
        // The opcode for RST looks like 11NNN111 where 00NNN000 is the address to jump to.
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

    void InstructionsDisplay::ensureInstructionIsDisplayed(word address)
    {
        int firstAddress = displayedInstructionsCache.front().address;
        int lastAddress = displayedInstructionsCache.back().address;

        // If the address is outside the instructions cache move immediately to the given address.
        if (address < firstAddress || address > lastAddress + 6)
        {
            moveTo(address);
            return;
        }

        // If the address is at the top of the displayed instructions list scroll up to center it.
        int firstDisplayedAddress = displayedInstructionsCache[firstDisplayedInstructionIndex].address;

        if (address <= firstDisplayedAddress + 6)
        {
            scrollUp(3);
            return;
        }

        // If the address is at the bottom of the displayed instructions list scroll down.
        std::size_t lastDisplayedIndex = 
            std::min<int>(firstDisplayedInstructionIndex + numberOfInstructionsDisplayed, displayedInstructionsCache.size() -1);

        int lastDisplayedAddress = 
            displayedInstructionsCache[lastDisplayedIndex].address;

        if (address >= lastDisplayedAddress - 6)
        {
            scrollDown(3);
            return;
        }
    }

    void InstructionsDisplay::moveSelectionUp()
    {
        if (selectedInstructionIndex > firstDisplayedInstructionIndex)
            selectedInstructionIndex--;
        else // try to scroll up if the currently selected instruction is the top one.
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
        else // try to scroll down if the currently selected instruction is the bottom one.
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

        // Make sure the selection is always in the list of displayed instructions.
        selectedInstructionIndex = std::max(selectedInstructionIndex, firstDisplayedInstructionIndex);
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

        // Make sure the selection is always in the list of displayed instructions.
        selectedInstructionIndex = std::min<unsigned int>(selectedInstructionIndex,
            firstDisplayedInstructionIndex + numberOfInstructionsDisplayed - 1);
    }

    bool InstructionsDisplay::getSelectedInstruction(Instruction& instruction)
    {
        if (selectedInstructionIndex >= displayedInstructionsCache.size())
            return false;

        instruction = displayedInstructionsCache[selectedInstructionIndex];
        return true;
    }

    unsigned int InstructionsDisplay::addInstructionToCache(word address)
    {
        Instruction instruction;

        instruction.address = address;
        instruction.opCode = memory.get(address);

        // For instructions of length 2 or 3 the proceeding bytes contain the 
        // parameters for the instruction. We pass the proceeding bytes to the 
        // function formatInstructionArguments regardless of the instruction length
        // byte2 and byte3 will only be used when necessary.
        byte byte2 = 0, byte3 = 0;

        if (static_cast<unsigned int>(address + 1) < memory.getTotalSize())
            byte2 = memory.get(address + 1);

        if (static_cast<unsigned int>(address + 2) < memory.getTotalSize())
            byte3 = memory.get(address + 2);

        instruction.arguments = formatInstructionArguments(instruction.opCode, byte2, byte3);

        displayedInstructionsCache.push_back(instruction);

        return address + instructionLengths[instruction.opCode];
    }

    unsigned int InstructionsDisplay::addInstructionsToCache(word startAddress, unsigned int number)
    {
        unsigned int address = startAddress;
        for (unsigned int i = 0; i < number; ++i)
        {            
            if (address >= memory.getTotalSize())
                break;

            address = addInstructionToCache(address);
        }

        return address;
    }

    void InstructionsDisplay::growCache(unsigned int number)
    {
        if (displayedInstructionsCache.empty())
            return;

        Instruction& lastInstruction = displayedInstructionsCache.back();

        unsigned int address = lastInstruction.address + instructionLengths[lastInstruction.opCode];

        // Check address has not overflowed to a number larger than the largest possible memory address (0xFFFF).
        if (address < memory.getTotalSize())
        {
            addInstructionsToCache(address, number);
        }
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

    ConsoleUI::ConsoleUI(Console& console_, DiagnosticCpu& cpu_, Memory& memory_):
        console(console_), cpu(cpu_), memory(memory_),
        instructionsDisplay(console_, cpu_, memory_)
    {}

    void ConsoleUI::notifyMemoryChanged()
    {
        instructionsDisplay.moveTo(cpu.getState().PC, false);
    }

    void ConsoleUI::draw()
    {
        std::size_t previousScreenBufferIndex = console.getActiveScreenBufferIndex();
        console.restoreDefaultScreenBuffer();

        console.clear();

        drawCpuState();
        drawHelp();
        drawDialog();

        console.setCursorPosition(0, console.getWindowSize().height);

        console.setActiveScreenBuffer(previousScreenBufferIndex);
    }

    void ConsoleUI::onConsoleEvent(const Console::Event& event)
    {
        if (event.isDirectInput)
        {
            if (!event.keyEvent.bKeyDown)
                return;

            const KEY_EVENT_RECORD& keyEvent = event.keyEvent;

            InstructionsDisplay::Instruction instruction;
            std::size_t index;
            switch (keyEvent.wVirtualKeyCode)
            {
                case 'R':
                    cpu.reset();

                    draw();
                    break;

                case VK_SPACE:
                    cpu.executeInstructionCycle();

                    draw();
                    break;

                case VK_TAB:
                    index = (console.getActiveScreenBufferIndex() + 1) % console.getNumberOfScreenBuffers();
                    console.setActiveScreenBuffer(index);
                        
                    break;

                case 'C':
                    beginDialog("Enter command:");
                    break;

                case 'B':    
                    if (instructionsDisplay.getSelectedInstruction(instruction))
                    {
                        cpu.toggleBreakpoint(instruction.address);
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
        else
        {
            endDialog();

            handleCommand(event.line);
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

        if (isInFollowMode)
            instructionsDisplay.ensureInstructionIsDisplayed(state.PC);

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
        short x = 54, y = 6;
        console.setCursorPosition(x, y++);
        console.write("q: Quit");

        console.setCursorPosition(x, y++);
        console.write("r: Reset");

        console.setCursorPosition(x, y++);
        console.write("space: Single step");

        console.setCursorPosition(x, y++);
        console.write("tab: View program output");

        console.setCursorPosition(x, y++);
        console.write("c: Enter command");

        console.setCursorPosition(x, y++);
        console.write("b: Toggle breakpoint");

        console.setCursorPosition(x, y++);
        if (isInFollowMode)
            console.write("f: Disable follow mode");
        else
            console.write("f: Enable follow mode");
    }

    void ConsoleUI::beginDialog(const std::string& prompt)
    {
        isInDialogMode = true;
        dialogPrompt = prompt;

        draw();

        console.setDirectInputMode(false);
    }

    void ConsoleUI::endDialog()
    {
        isInDialogMode = false;
        draw();

        console.setDirectInputMode(true);
    }

    void ConsoleUI::drawDialog()
    {
        if (isInDialogMode)
        {
            short height = console.getWindowSize().height;

            console.setCursorPosition(0, height - 1);
            console.write(dialogPrompt);

            console.setCursorPosition(0, height);
        }
    }

    void ConsoleUI::showMessage(const std::string& message)
    {
        draw();

        console.setCursorPosition(0, console.getWindowSize().height);
        console.write(message);
    }

    void ConsoleUI::handleCommand(const std::string& command)
    {
        std::smatch match;

        // Handle commands of the form: <command> <address>
        
        std::regex commandWithAddressRegex(R"--(\s*(\w+)\s+([0-9|a-f]{1,4})\s*)--",
             std::regex_constants::icase);
        
        if (std::regex_match(command, match, commandWithAddressRegex))
        {
            if (match.size() != 3)
            {
                showMessage("Command not recognised.");
                return;
            }

            std::string commandString(match[1].length(), ' ');
            std::transform(match[1].first, match[1].second, commandString.begin(), 
                [] (char c) { return std::tolower(c); });

            // Because match[1] matches the hex number pattern specified regex it must be well-formed.
            word address = std::stoi(match[2], nullptr, 16);

            if (commandString == "breakpoint")
            {
                cpu.toggleBreakpoint(address);

                draw();
                return;
            }

            if (commandString == "goto" || commandString == "move")
            {
                cpu.setProgramCounter(address);

                draw();
                return;
            }

            if (commandString == "view")
            {
                instructionsDisplay.moveTo(address);

                draw();
                return;
            }
        }     

        // Handle commands of the form: <command> <filename>

        std::regex commandWithFilenameRegex(R"--(\s*(\w+)\s+((\w+)(.\w+)?)\s*)--",
            std::regex_constants::icase);

        if (std::regex_match(command, match, commandWithFilenameRegex))
        {
            if (match.size() != 5)
            {
                showMessage("Command not recognised.");
                return;
            }

            std::string commandString(match[1].length(), ' ');
            std::transform(match[1].first, match[1].second, commandString.begin(), 
                [] (char c) { return std::tolower(c); });

            if (commandString == "save")
            {
                cpu.saveBreakpoints(match[2]);

                draw();                
                return;
            }

            if (commandString == "load")
            {
                cpu.loadBreakpoints(match[2]);

                draw();
                return;
            }
        }

        showMessage("Command not recognised.");
        return;
    }

} // namespace emulator
