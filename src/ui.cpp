#include "ui.hpp"

namespace emulator
{
    UI::UI(): window(sf::VideoMode(800, 600), "intel 8080 emulator - Space invaders")
    {}

    void UI::run()
    {
        window.setFramerateLimit(30);
        
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
            }
        }
    }

} // namespace emulator
