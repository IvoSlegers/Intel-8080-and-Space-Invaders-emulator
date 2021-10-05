#include "spaceinvaders_application.hpp"

#include "to_hex_string.hpp"

namespace emulator
{
    SpaceInvadersApplication::SpaceInvadersApplication(): memory(0x2000, 0x2000), io(), cpu(memory, io),
        window(sf::VideoMode(SpaceInvadersVideo::crtHeight * SpaceInvadersVideo::scalingFactor + 100, SpaceInvadersVideo::crtWidth * SpaceInvadersVideo::scalingFactor + 100), "intel 8080 - Space Invaders"), //console(),
        /* consoleUI(console, cpu, memory),*/ video(window, memory)
    {}

    void SpaceInvadersApplication::run()
    {
        memory.loadMemoryFromFile("invaders.rom");
        //consoleUI.notifyMemoryChanged();

        window.setFramerateLimit(240);

        //consoleUI.draw();
        
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
        //consoleUI.draw();
    }

    void SpaceInvadersApplication::quit()
    {
        window.close();
    }

    // void SpaceInvadersApplication::onConsoleEvent(const Console::Event& event)
    // {
    //     if (event.isDirectInput && event.keyEvent.wVirtualKeyCode == VK_ESCAPE)
    //         window.close();

    //     consoleUI.onConsoleEvent(event);
    // }

    void SpaceInvadersApplication::onEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::Closed)
            quit();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            quit();

        // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
        //     window.close();

        // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
        // {
        //     cpu.executeInstructionCycle();
        //     consoleUI.draw();
        // }
    }

    void SpaceInvadersApplication::handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
            onEvent(event);

        // Console::Event consoleEvent;
        // while (console.pollEvent(consoleEvent))
        //     onConsoleEvent(consoleEvent);
    }

    void SpaceInvadersApplication::update(float delta)
    {
        machineCyclesToBeExecuted += delta * 2'000'000;

        while (machineCyclesToBeExecuted > 0)
        {
            machineCyclesToBeExecuted -= cpu.executeInstructionCycle();
        }

        if (screenTimer.getElapsedTime().asSeconds() > 1/120.0f)
        {
            if (upperHalf)
            {
                video.update(0, 0, SpaceInvadersVideo::crtWidth, SpaceInvadersVideo::crtHeight/2,
                sf::Color::White, sf::Color::Black);
                cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST1);
            }
            else
            {
                video.update(0, SpaceInvadersVideo::crtHeight/2, SpaceInvadersVideo::crtWidth,
                    SpaceInvadersVideo::crtHeight/2, sf::Color::White, sf::Color::Black);
                cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST2);
            }

            screenTimer.restart();
            upperHalf = !upperHalf;
        }

        // machineCyclesToBeExecuted = 2'000'000/120 + 1;
        // while (machineCyclesToBeExecuted > 0)
        // {
        //     machineCyclesToBeExecuted -= cpu.executeInstructionCycle();                
        // }

        // video.update(0, 0, SpaceInvadersVideo::crtWidth, SpaceInvadersVideo::crtHeight/2,
        // sf::Color::White, sf::Color::Black);
        // cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST1);

        // machineCyclesToBeExecuted = 2'000'000/120 + 1;
        // while (machineCyclesToBeExecuted > 0)
        // {
        //     machineCyclesToBeExecuted -= cpu.executeInstructionCycle();                
        // }

        // video.update(0, SpaceInvadersVideo::crtHeight/2, SpaceInvadersVideo::crtWidth,
        //     SpaceInvadersVideo::crtHeight/2, sf::Color::White, sf::Color::Black);
        // cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST2);    
    }

    void SpaceInvadersApplication::draw()
    {
        window.clear(sf::Color::Black);

        video.draw();

        window.display();
    }
} // namespace emulator
