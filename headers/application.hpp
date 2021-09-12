#pragma once

#include "memory.hpp"
#include "cpu.hpp"
#include "io.hpp"

#include "consolegui/console.hpp"
#include "console_ui.hpp"
#include "ui.hpp"

#include <SFML/Graphics.hpp>

namespace emulator
{
    class Application
    {
        public:
            explicit Application();

            void run();

        private:
            void onConsoleKeyEvent(const KEY_EVENT_RECORD& event);
            void onEvent(const sf::Event& event);

            void handleEvents();
            void update(float delta);
            void draw();

            Memory memory;
            EmptyIO io;
            Cpu cpu;

            sf::RenderWindow window;
            Console console;

            ConsoleUI consoleUI;
    };
} // namespace emulator
