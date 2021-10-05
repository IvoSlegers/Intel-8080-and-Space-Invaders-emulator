#pragma once

#include "int_types.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

namespace emulator
{
    class Memory;

    /*
        Class for emulating the video system of the space invaders cabinet.
        Handles the interaction between the video buffer in the memory of the i8080 system
        and the SFML window.

        The video buffer is located at 2400 - 3FFF in the memory. Each bit encodes a pixel being
        either on (1) or off (0).
    */
    class SpaceInvadersVideo
    {
        public:
            SpaceInvadersVideo(sf::RenderWindow& window, Memory& memory);

            // Copies/translates the region in the video buffer of the i8080 system that corresponds to the 
            // rectangle determined by (x, y, width, height) to the SFML/OpenGL texture.
            // A pixel that is on will be drawn in the forground color, a pixel that is off will be drawn
            // in the background color.
            void update(unsigned short x, unsigned short y, unsigned short width, unsigned short height,
                sf::Color foregroundColor, sf::Color backgroundColor);

            // Draw the SFML texture to the screen.
            void draw();

            // The resolution of the CRT in the space invaders cabinet is 256x224
            static constexpr unsigned short crtWidth = 256;
            static constexpr unsigned short crtHeight = 224;

            // We scale the 256x244 resolution up by scalingFactor
            static constexpr float scalingFactor = 3;

            // Optimal size for the SFML window needed to contain the video display.
            // Note that the CRT in the cabinet is rorated 90 degrees. Hence
            // the optimal window width depends on the crt height and vice versa.
            static constexpr unsigned short optimalWindowWidth = scalingFactor * crtHeight + 100;
            static constexpr unsigned short optimalWindowHeight = scalingFactor * crtWidth + 100;

            // The in memory video buffer is located at addresses 2400 - 3FFF
            static constexpr unsigned short videoBufferAddress = 0x2400;
        private:
            sf::RenderWindow& window;

            sf::Texture texture;
            sf::Sprite sprite;
            std::unique_ptr<byte[]> videoData;
            bool textureUpdateRequired = true;

            sf::RectangleShape outline;

            Memory& memory;
    };
} // namespace emulator
