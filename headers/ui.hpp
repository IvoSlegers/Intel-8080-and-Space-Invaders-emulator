#pragma once

#include <SFML/Window.hpp>

namespace emulator
{
    class UI
    {
        public:
            explicit UI();

            void run();

        private:
            sf::Window window;
    };
} // namespace emulator
