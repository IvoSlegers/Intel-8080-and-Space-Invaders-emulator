#include "spaceinvaders_application.hpp"

#include "to_hex_string.hpp"

namespace emulator
{
    SpaceInvadersApplication::SpaceInvadersApplication(): memory(0x2000, 0x2000), io(), cpu(memory, io),
        window(sf::VideoMode(SpaceInvadersVideo::optimalWindowWidth, SpaceInvadersVideo::optimalWindowHeight), 
                "intel 8080 - Space Invaders"),
        video(window, memory)
    {}

    void SpaceInvadersApplication::run()
    {
        memory.loadMemoryFromFile("roms/invaders.rom");

        window.setFramerateLimit(240);
        
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

    void SpaceInvadersApplication::reset()
    {
        cpu.reset();
    }

    void SpaceInvadersApplication::quit()
    {
        window.close();
    }

    void SpaceInvadersApplication::onEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::Closed)
            quit();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            quit();
    }

    void SpaceInvadersApplication::handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
            onEvent(event);
    }

    void SpaceInvadersApplication::update(float delta)
    {
        // By default the intel 8080 processor runs at 2 MHz.
        machineCyclesToBeExecuted += delta * 2'000'000;

        while (machineCyclesToBeExecuted > 0)
        {
            machineCyclesToBeExecuted -= cpu.executeInstructionCycle();
        }

        // The CRT in the space invaders cabinet had a refresh rate of 60Hz.
        // Hence the frequency of either a RST1 or RST2 happing is 120Hz.
        if (screenTimer.getElapsedTime().asSeconds() > 1/120.0f)
        {
            if (upperHalf)
            {
                video.update(0, 0, SpaceInvadersVideo::crtWidth, SpaceInvadersVideo::crtHeight/2,
                sf::Color::White, sf::Color::Black);

                // The Space Invaders cabinet issues a RST1 interrupt each time the top half of the
                // screen is drawn by the CRT.
                cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST1);
            }
            else
            {
                video.update(0, SpaceInvadersVideo::crtHeight/2, SpaceInvadersVideo::crtWidth,
                    SpaceInvadersVideo::crtHeight/2, sf::Color::White, sf::Color::Black);

                // The Space Invaders cabinet issues a RST2 interrupt each time the bottom half of the 
                // screen is drawn by the CRT.
                cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST2);
            }

            screenTimer.restart();
            upperHalf = !upperHalf;
        }
    }

    void SpaceInvadersApplication::draw()
    {
        window.clear(sf::Color::Black);

        video.draw();

        window.display();
    }
} // namespace emulator
