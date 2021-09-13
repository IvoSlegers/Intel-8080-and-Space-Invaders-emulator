#include "spaceinvaders_video.hpp"

#include <SFML/Graphics.hpp>

namespace emulator
{
    SpaceInvadersVideo::SpaceInvadersVideo(sf::RenderWindow& window_, Memory& memory_): 
        window(window_), texture(), sprite(texture), memory(memory_)
    {
        texture.create(width, height);
        sprite.setPosition(100, 100);

        videoData = std::make_unique<byte[]>(static_cast<std::size_t>(width * height));

        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                videoData[width * i + j] = i;
                videoData[width * i + j + 1] = j;
                videoData[width * i + j + 2] = 128;
            }
        }
    }

    void SpaceInvadersVideo::draw()
    {
        if (videoDataUpdateRequired)
        {
            texture.update(videoData.get());
            videoDataUpdateRequired = false;
        }
        
        window.draw(sprite);
    }

} // namespace emulator
