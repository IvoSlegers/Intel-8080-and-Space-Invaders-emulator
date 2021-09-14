#pragma once

#include "int_types.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

namespace emulator
{
    class Memory;

    class SpaceInvadersVideo
    {
        public:
            SpaceInvadersVideo(sf::RenderWindow& window, Memory& memory);

            void update(unsigned short x, unsigned short y, unsigned short width, unsigned short height,
                sf::Color foregroundColor, sf::Color backgroundColor);

            void draw();

            // The resolution of the CRT in the space invaders cabinet is 256x224
            static constexpr unsigned short crtWidth = 256;
            static constexpr unsigned short crtHeight = 224;

            // The video buffer is located at addresses 2400 - 3FFF
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
