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
    class SpaceInvadersApplication : public Application
    {
        public:
            explicit SpaceInvadersApplication();

            // Run the application.
            void run() override;

        private:
            void onEvent(const sf::Event& event);

            void handleEvents();
            void update(float delta);
            void draw();

            void reset();
            void quit();

            Memory memory;
            SpaceInvadersIO io;
            DiagnosticCpu cpu;

            sf::RenderWindow window;

            SpaceInvadersVideo video;

            int machineCyclesToBeExecuted = 0;
            sf::Clock screenTimer;

            // The Space Invaders game updates the top and bottom halves of the screen at different times
            // This flag records which has last been updated. 
            bool upperHalf = true;
    };
} // namespace emulator
