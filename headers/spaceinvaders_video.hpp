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

            void draw();

            // The resolution of the CRT in the space invaders cabinet is 256x224
            static constexpr unsigned short width = 256;
            static constexpr unsigned short height = 244;
        private:
            sf::RenderWindow& window;

            sf::Texture texture;
            sf::Sprite sprite;
            std::unique_ptr<byte[]> videoData;
            bool videoDataUpdateRequired = true;

            Memory& memory;
    };
} // namespace emulator
