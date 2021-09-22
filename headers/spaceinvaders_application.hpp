#pragma once

#include "application.hpp"
#include "memory.hpp"
#include "diagnostic_cpu.hpp"
#include "spaceinvaders_io.hpp"

#include "consolegui/console.hpp"
#include "console_ui.hpp"
#include "spaceinvaders_video.hpp"

#include <SFML/Graphics.hpp>

#include <set>

namespace emulator
{
    class SpaceInvadersApplication
    {
        public:
            explicit SpaceInvadersApplication();

            void run();

            void reset();
            void quit();
            void executeSingleStep();
            void toggleBreakpoint(word address);

        private:
            void onConsoleKeyEvent(const Console::Event& event);
            void onEvent(const sf::Event& event);

            void handleEvents();
            void update(float delta);
            void draw();

            Memory memory;
            SpaceInvadersIO io;
            DiagnosticCpu cpu;

            sf::RenderWindow window;
            Console console;

            ConsoleUI consoleUI;
            SpaceInvadersVideo video;

            int machineCyclesToBeExecuted = 0;
            sf::Clock screenTimer;
            bool upperHalf = true;

            bool running = false;
            void triggerBreakpoint();

            std::set<word> breakpoints;

            friend ConsoleUI;
            friend InstructionsDisplay;
    };
} // namespace emulator
