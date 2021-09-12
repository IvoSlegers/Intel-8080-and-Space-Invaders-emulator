#pragma once

#include <SFML/Graphics.hpp>

namespace emulator
{
    class Application;

    class UI
    {
        public:
            explicit UI();

            void run(Application& parentApplication);

        private:
            void draw();
            
            sf::RenderWindow window;
    };
} // namespace emulator
