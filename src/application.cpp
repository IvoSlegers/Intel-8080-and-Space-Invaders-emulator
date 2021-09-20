#include "application.hpp"

#include "to_hex_string.hpp"

#include <iostream>

namespace emulator
{
    Application::Application(): memory(0x2000, 0x2000), io(), cpu(memory, io),
        window(sf::VideoMode(800, 600), "intel 8080 - Space Invaders"), console(),
        consoleUI(*this, console), video(window, memory)
    {}

    void Application::run()
    {
        memory.loadROMFromFile("invaders.rom");

        window.setFramerateLimit(60);

        consoleUI.initialise();
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

    void Application::reset()
    {
        cpu.reset();
        consoleUI.draw();
    }

    void Application::quit()
    {
        window.close();
    }

    void Application::executeSingleStep()
    {
        cpu.executeInstructionCycle();
        consoleUI.draw();
    }

    void Application::toggleBreakpoint(word address)
    {
        std::set<word>::iterator i = breakpoints.find(address);

        if (i == breakpoints.end())
            breakpoints.insert(address);           
        else
            breakpoints.erase(i);           
    }

    void Application::onConsoleKeyEvent(const Console::Event& event)
    {
        consoleUI.onConsoleKeyEvent(event);
    }

    void Application::onEvent(const sf::Event& event)
    {
        // if (event.type == sf::Event::Closed)
        //     window.close();

        // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
        //     window.close();

        // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
        // {
        //     cpu.executeInstructionCycle();
        //     consoleUI.draw();
        // }
    }

    void Application::handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
            onEvent(event);

        Console::Event consoleEvent;
        while (console.pollEvent(consoleEvent))
            onConsoleKeyEvent(consoleEvent);
    }

    void Application::update(float delta)
    {
        // machineCyclesToBeExecuted += delta * 2'000'000;

        // while (machineCyclesToBeExecuted > 0)
        // {
        //     machineCyclesToBeExecuted -= cpu.executeInstructionCycle();
        // }

        // if (screenTimer.getElapsedTime().asSeconds() > 1/120.0f)
        // {
        //     if (upperHalf)
        //     {
        //         video.update(0, 0, SpaceInvadersVideo::crtWidth, SpaceInvadersVideo::crtHeight/2,
        //         sf::Color::White, sf::Color::Black);
        //         cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST1);
        //     }
        //     else
        //     {
        //         video.update(0, SpaceInvadersVideo::crtHeight/2, SpaceInvadersVideo::crtWidth,
        //             SpaceInvadersVideo::crtHeight/2, sf::Color::White, sf::Color::Black);
        //         cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST2);
        //     }

        //     screenTimer.restart();
        //     upperHalf = !upperHalf;
        // }

        if (false)
        {
            machineCyclesToBeExecuted = 2'000'000/120;
            while (machineCyclesToBeExecuted > 0 && running)
            {
                machineCyclesToBeExecuted -= cpu.executeInstructionCycle();
                if (breakpoints.count(cpu.getState().PC) != 0)
                {
                    triggerBreakpoint();
                    return;
                }                    
            }

            video.update(0, 0, SpaceInvadersVideo::crtWidth, SpaceInvadersVideo::crtHeight/2,
            sf::Color::White, sf::Color::Black);
            cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST1);

            machineCyclesToBeExecuted = 2'000'000/120;
            while (machineCyclesToBeExecuted > 0 && running)
            {
                machineCyclesToBeExecuted -= cpu.executeInstructionCycle();
                if (breakpoints.count(cpu.getState().PC) != 0)
                {
                    triggerBreakpoint();
                    return;
                }
                    
            }

            video.update(0, SpaceInvadersVideo::crtHeight/2, SpaceInvadersVideo::crtWidth,
                SpaceInvadersVideo::crtHeight/2, sf::Color::White, sf::Color::Black);
            cpu.issueRSTInterrupt(Cpu::RestartInstructions::RST2);
        }        
    }

    void Application::draw()
    {
        window.clear(sf::Color::Black);

        video.draw();

        window.display();
    }

    void Application::triggerBreakpoint()
    {
        consoleUI.draw();
        running = false;
    }

} // namespace emulator
