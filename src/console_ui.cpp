#include "console_ui.hpp"

#include "cpu.hpp"
#include "memory.hpp"
#include "opcode_info.hpp"
#include "to_hex_string.hpp"

#include "consolegui\console.hpp"

namespace emulator
{
    ConsoleUI::ConsoleUI(Console& console_, const Cpu& cpu_, const Memory& memory_): 
        console(console_), cpu(cpu_), memory(memory_)
    {}

    void ConsoleUI::draw()
    {
        console.clear();
        drawCpuState();
        console.setCursorPosition(0, console.getScreenSize().heigh);
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

        drawInstructions(state.PC, 16, 0, 6);
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

    void ConsoleUI::drawInstructions(word address, word number, short x, short y)
    {
        int i = 0;
        while (address < memory.getTotalSize() && number > 0)
        {
            address += drawInstruction(address, x, y + i);

            ++i;
            --number;
        }
    }

    word ConsoleUI::drawInstruction(word address, short x, short y)
    {
        bool isCurrentInstruction = address == cpu.getState().PC;

        Color foregroundColor = isCurrentInstruction ? Color::Black : Color::White;
        Color backgroundColor = isCurrentInstruction ? Color::White : Color::Black;

        console.setTextColor(foregroundColor, backgroundColor);

        if (isCurrentInstruction)
        {
            console.setCursorPosition(x, y);
            console.write("                                                ");
        }

        console.setCursorPosition(x, y);
        console.write(toHexString(address));

        byte opCode = memory.get(address);

        console.setTextColor(Color::Blue, backgroundColor);

        console.setCursorPosition(x + 6, y);
        console.write(nmemonics[opCode]);

        console.setTextColor(foregroundColor, backgroundColor);

        byte byte2 = 
            (static_cast<std::size_t>(address + 1) < memory.getTotalSize()) ? memory.get(address + 1) : 0;
        byte byte3 = 
            (static_cast<std::size_t>(address + 2) < memory.getTotalSize()) ? memory.get(address + 2) : 0;

        console.setCursorPosition(x + 14, y);
        console.write(formatInstructionArguments(opCode, byte2, byte3));

        return instructionLengths[opCode];
    }
} // namespace emulator
