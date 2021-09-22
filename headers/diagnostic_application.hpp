#pragma once

#include "application.hpp"

#include "consolegui/console.hpp"
#include "memory.hpp"
#include "diagnostic_cpu.hpp"
#include "io.hpp"
#include "console_ui.hpp"

#include <string>

namespace emulator
{
    class DiagnosticApplication : public Application
    {
        public:
            static constexpr std::size_t romSize = 0;
            static constexpr std::size_t ramSize = 0x10000;

            explicit DiagnosticApplication();
            virtual ~DiagnosticApplication();

            virtual void run();

        private:
            void runDiagnostic(const std::string& filename);

            void printOutput();

            void handleEvents();
            void onConsoleEvent(const Console::Event& event);

            Console console;

            Memory memory;
            EmptyIO io;
            DiagnosticCpu cpu;  

            ConsoleUI consoleUI;   

            bool running = true;       
    };
} // namespace emulator
