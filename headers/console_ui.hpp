#pragma once

#include "int_types.hpp"

#include <string>

namespace emulator
{
    class Console;
    class Memory;
    struct Cpu;

    class ConsoleUI
    {
        public:
            explicit ConsoleUI(Console& console, const Cpu& cpu, const Memory& memory);

            void draw();

            void beginDialog(const std::string& prompt);
            std::string endDialog();

            bool isInDialogMode() const { return isInDialogMode_; }

        private:           
            void drawCpuState();

            void drawRegister(const std::string& name, byte value, short x, short y);
            void drawWordRegister(const std::string& name, word value, short x, short y);
            void drawFlag(const std::string& name, bool value, short x, short y);

            void drawInstructions(word address, word len, short x, short y);
            word drawInstruction(word address, short x, short y);

            void drawHelp();

            Console& console;

            bool isInDialogMode_ = false;

            const Cpu& cpu;
            const Memory& memory;
    };
} // namespace emulator
