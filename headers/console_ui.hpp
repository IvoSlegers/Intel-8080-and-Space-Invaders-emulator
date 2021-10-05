#pragma once

#include "int_types.hpp"

#include "consolegui/console.hpp"
using Console = console::Console;

#include <vector>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace emulator
{
    class Memory;
    class DiagnosticCpu;

    /*
        Class responsible for displaying part of the instructions in the memory of the i8080 to the console.

        Supports moving through the instructions in memory, selecting instructions and
        jumping to addresses in memory specified by control flow instructions.

        Privately maintains a cache with text needed to displayed currently visible instructions.
    */
    class InstructionsDisplay
    {
        public:
            // Struct to hold all information needed to display a i8080 instruction on the console.
            struct Instruction
            {
                word address;
                byte opCode;
                std::string arguments;
            };

            explicit InstructionsDisplay(Console& console, DiagnosticCpu& cpu,
                Memory& memory);

            // Draw the contents of the display to the console at location (x, y).
            void draw(short x, short y);

            // Sets region in memory to be displayed.
            // The addToHistory argument controls whether the location will be added to the undo stack.
            void moveTo(word address, bool addToHistory = true);

            // Moves to the region in memory, pointed at by the given control flow instruction.
            // Valid instructions are (conditional) RET, CALL, JMP and RST instructions.
            // returns true if the given instruction is a valid control flow instruction, false otherwise.
            bool moveToTarget(const Instruction& instruction);

            // Moves the display back to the previously visited memory address.
            void moveBack();

            // Move the region of memory displayed to make sure the instruction at the given address
            // is visible.
            void ensureInstructionIsDisplayed(word address);

            void moveSelectionUp();
            void moveSelectionDown();

            // Scrolls the region of memory displayed.
            void scrollUp(unsigned short lines = 1);
            void scrollDown(unsigned short lines = 1);

            // If an instruction is selected puts it in the instruction argument and returns true.
            // Returns false otherwise.
            bool getSelectedInstruction(Instruction& instruction);

        private:
            static const unsigned short numberOfInstructionsDisplayed = 16;

            Console& console;

            DiagnosticCpu& cpu;
            Memory& memory;

            // Adds instruction at address in memory to displayed instructions cache.
            // Returns address of instruction proceeding this instruction.
            unsigned int addInstructionToCache(word address);

            // If available Adds multiple instructions to displayed instructions cache. First instruction
            // is found at address.
            // Returns address of first instruction that was not added.
            unsigned int addInstructionsToCache(word address, unsigned int number);

            // If available adds specified number of instructions following the last cached instruction
            // to the displayed instructions cache.
            void growCache(unsigned int number);

            void drawInstructions(unsigned int cacheStartIndex, short x, short y);
            void drawInstruction(const Instruction& instruction, bool isSelected, bool isCurrent, bool isBreakpoint, short x, short y);

            std::vector<Instruction> displayedInstructionsCache;
            unsigned int firstDisplayedInstructionIndex = 0;
            unsigned int selectedInstructionIndex = 0;

            std::vector<word> jumpHistory;
    };

    /*
        Class responsible for displaying the state of the cpu on the console.

        Allows for the user to inspect the state of the cpu and the instructions waiting to be executed.
        The user can also control the cpu, add breakpoints, save breakpoints to a file and load them.
    */
    class ConsoleUI
    {
        public:
            explicit ConsoleUI(Console& console, DiagnosticCpu& cpu, Memory& memory);

            // Notifies the UI that the memory of the i8080 has been changed by outside sources
            // hence the instruction cache must be refreshed.
            void notifyMemoryChanged();

            // Draws the Cpu UI to the console.
            void draw();

            // Notify the console UI that a console event has taken place.
            void onConsoleEvent(const Console::Event& event);

        private: 
            void drawCpuState();

            void drawRegister(const std::string& name, byte value, short x, short y);
            void drawWordRegister(const std::string& name, word value, short x, short y);
            void drawFlag(const std::string& name, bool value, short x, short y);            

            void drawHelp();

            void beginDialog(const std::string& prompt);
            void endDialog();
            void drawDialog();

            void showMessage(const std::string& message);

            void handleCommand(const std::string& command);

            Console& console;

            DiagnosticCpu& cpu;
            Memory& memory;

            InstructionsDisplay instructionsDisplay;

            bool isInDialogMode = false;
            std::string dialogPrompt;

            // When the UI is in follow mode it will make use the instruction pointed at
            // by the PC register is always visible.
            bool isInFollowMode = true;
    };
} // namespace emulator
