#pragma once

#include "int_types.hpp"
#include "consolegui/console.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <functional>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace emulator
{
    class Memory;
    class DiagnosticCpu;

    class InstructionsDisplay
    {
        public:
            struct Instruction
            {
                word address;
                byte opCode;
                std::string arguments;
            };

            explicit InstructionsDisplay(Console& console, DiagnosticCpu& cpu,
                Memory& memory);

            void draw(short x, short y);

            void moveTo(word address, bool addToHistory = true);
            bool moveToTarget(const Instruction& instruction);
            void moveBack();

            void assureInstructionIsDisplayed(word address);

            void moveSelectionUp();
            void moveSelectionDown();

            void scrollUp(unsigned short lines = 1);
            void scrollDown(unsigned short lines = 1);

            bool getSelectedInstruction(Instruction& instruction);

        private:
            static const unsigned short numberOfInstructionsDisplayed = 16;

            Console& console;

            DiagnosticCpu& cpu;
            Memory& memory;

            unsigned int addInstructionToCache(word address);
            unsigned int addInstructionsToCache(word address, unsigned int number);
            void growCache(unsigned int number);

            void drawInstructions(unsigned int cacheStartIndex, short x, short y);
            void drawInstruction(const Instruction& instruction, bool isSelected, bool isCurrent, bool isBreakpoint, short x, short y);

            std::vector<Instruction> displayedInstructionsCache;
            unsigned int firstDisplayedInstructionIndex = 0;
            unsigned int selectedInstructionIndex = 0;

            std::vector<word> jumpHistory;
    };

    class ConsoleUI
    {
        public:
            explicit ConsoleUI(Console& console, DiagnosticCpu& cpu, Memory& memory);

            void signalMemoryChanged();
            void draw();

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
            std::stringstream dialogInput;

            bool isInFollowMode = true;
    };
} // namespace emulator
