#pragma once

#include "application.hpp"

#include "consolegui/console.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "io.hpp"

#include <string>

namespace emulator
{
    class DiagnosticsApplication : public Application
    {
        public:
            static constexpr std::size_t romSize = 0;
            static constexpr std::size_t ramSize = 0x10000;

            explicit DiagnosticsApplication();
            virtual ~DiagnosticsApplication();

            virtual void run();

        private:
            void runDiagnostic(const std::string& filename);

            void printOutput();

            Console console;

            Memory memory;
            EmptyIO io;
            Cpu cpu;            
    };
} // namespace emulator
