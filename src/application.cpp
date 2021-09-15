#include "application.hpp"

#include "to_hex_string.hpp"

#include <iostream>
#include <stdlib.h>

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

        for (int i = 0x2400; i < 0x3fff; ++i)
        {
            memory[i] = rand() % 256;
        }

        try
        {
            for (int i = 0; i < 50'000; ++i)
            {
                cpu.executeInstructionCycle();
            }
        }
        catch (EmulatorException& exception)
        {
            consoleUI.draw();
            throw;
        }

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
        if (event.uChar.AsciiChar == 'q')
            window.close();

        if (event.uChar.AsciiChar == 'r')
            cpu.reset();

        if (event.uChar.AsciiChar == 's' && event.bKeyDown)
        {
            cpu.executeInstructionCycle();
            consoleUI.draw();
        }

        if (event.uChar.AsciiChar == 'f' && event.bKeyDown)
        {
            for (int i = 0; i < 100; ++i)
                cpu.executeInstructionCycle();
            consoleUI.draw();
        }
    }

    void Application::onEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
        {
            cpu.executeInstructionCycle();
            consoleUI.draw();
        }
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
        video.update(0, 0, SpaceInvadersVideo::crtWidth, SpaceInvadersVideo::crtHeight, sf::Color::Red, sf::Color::Blue);
    }

    void Application::draw()
    {
        window.clear(sf::Color::Black);

        video.draw();

        window.display();
    }

} // namespace emulator
