#pragma once

#include "memory.hpp"
#include "cpu.hpp"
#include "io.hpp"

#include "console_ui.hpp"
#include "ui.hpp"

namespace emulator
{
    class Application
    {
        public:
            explicit Application();

            void run();

        private:
            bool onKeyEvent(const KEY_EVENT_RECORD& event);

            Memory memory;
            EmptyIO io;
            Cpu cpu;

            ConsoleUI consoleUI;
            UI ui;
    };
} // namespace emulator
