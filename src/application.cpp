#include "application.hpp"

#include "to_hex_string.hpp"

#include <iostream>

namespace emulator
{
    Application::Application(): memory(0x2000, 0x2000), io(), cpu(memory, io),
        window(sf::VideoMode(800, 600), "intel 8080 - Space Invaders"), console(),
        consoleUI(console, cpu, memory), video(window, memory)
    {}

    void Application::run()
    {
        memory.loadROMFromFile("invaders.rom");

        window.setFramerateLimit(30);

        consoleUI.draw();
        
        sf::Clock frametimeClock;
        while (window.isOpen())
        {
            handleEvents();

            float delta = frametimeClock.getElapsedTime().asSeconds();
            frametimeClock.restart();

            update(delta);

            draw();
        }
    }

    void Application::onConsoleKeyEvent(const KEY_EVENT_RECORD& event)
    {
    }

    void Application::onEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    void Application::handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
            onEvent(event);

        KEY_EVENT_RECORD consoleEvent;
        while (console.pollEvent(consoleEvent))
            onConsoleKeyEvent(consoleEvent);
    }

    void Application::update(float delta)
    {

    }

    void Application::draw()
    {
        window.clear(sf::Color::Black);

        //video.draw();

        window.display();
    }

} // namespace emulator
