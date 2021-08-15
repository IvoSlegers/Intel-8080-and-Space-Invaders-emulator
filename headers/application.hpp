#pragma once

#include "consolegui\console.hpp"
#include "memory.hpp"
#include "cpu.hpp"

namespace emulator
{
    class Application
    {
        public:
            Application();

            void run();

        private:
            void drawCpuState();

            void drawRegister(const std::string& name, byte value, short x, short y);
            void drawWordRegister(const std::string& name, word value, short x, short y);
            void drawFlag(const std::string& name, bool value, short x, short y);

            void drawInstructions(word address, word len, short x, short y);
            word drawInstruction(word address, short x, short y);

            Memory memory;
            Cpu cpu;

            Console console;
    };
} // namespace emulator
