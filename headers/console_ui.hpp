#pragma once

#include "consolegui\console.hpp"
#include "int_types.hpp"

namespace emulator
{
    class Memory;
    struct Cpu;

    class ConsoleUI
    {
        public:
            explicit ConsoleUI(const Cpu& state, const Memory& memory);

            void draw();

        private:           
            void drawCpuState();

            void drawRegister(const std::string& name, byte value, short x, short y);
            void drawWordRegister(const std::string& name, word value, short x, short y);
            void drawFlag(const std::string& name, bool value, short x, short y);

            void drawInstructions(word address, word len, short x, short y);
            word drawInstruction(word address, short x, short y);

            const Cpu& cpu;
            const Memory& memory;

            Console console;
    };
} // namespace emulator
