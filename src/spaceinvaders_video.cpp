#include "spaceinvaders_video.hpp"

#include "emulator_exception.hpp"
#include "memory.hpp"

#include <SFML/Graphics.hpp>

namespace emulator
{
    SpaceInvadersVideo::SpaceInvadersVideo(sf::RenderWindow& window_, Memory& memory_): 
        window(window_), memory(memory_)
    {
        texture.create(crtWidth, crtHeight);
        texture.setSmooth(false);

        sprite.setTexture(texture);

        sprite.setPosition(50, 50 + scalingFactor * crtWidth);
        sprite.setScale(sf::Vector2f(scalingFactor, scalingFactor));

        // The CRT in the Space Invaders cabinet was rotated 90 degrees counter-clockwise.
        sprite.setRotation(-90.f);

        // The SFML texture by default takes data buffer with RGBA pixels.
        videoData = std::make_unique<byte[]>(static_cast<std::size_t>(4 * crtWidth * crtHeight));

        outline.setOutlineColor(sf::Color::White);
        outline.setOutlineThickness(1.0f);
        outline.setPosition(sf::Vector2f(49, 49));
        outline.setSize(sf::Vector2f(scalingFactor * crtHeight + 2, scalingFactor * crtWidth + 2));
    }

    void SpaceInvadersVideo::update(unsigned short x, unsigned short y, 
        unsigned short width, unsigned short height,
        sf::Color foregroundColor, sf::Color backgroundColor)
    {
        if (x < 0 || (x + width) > crtWidth || y < 0 || (y + height) > crtHeight)
            throw EmulatorException("Invalid update rectangle specified in SpaceInvadersVideo::update.");

        for (unsigned short j = 0; j < height; ++j)
        {
            unsigned short pixelY = y + j;

            for (unsigned short i = 0; i < width; ++i)
            {
                unsigned short pixelX = x + i;                
                unsigned short pixelNumber = pixelY * crtWidth + pixelX;

                // Each byte encodes 8 pixels.
                word address = videoBufferAddress + (pixelNumber / 8);
                byte bitNumber = pixelNumber % 8;

                byte value = memory.get(address);
                bool pixelEnabled = (value >> (bitNumber)) & 1;

                sf::Color color = pixelEnabled ? foregroundColor : backgroundColor;

                unsigned int index = 4 * pixelNumber;
                
                videoData[index] = color.r;
                videoData[index + 1] = color.g;
                videoData[index + 2] = color.b;
                videoData[index + 3] = color.a;
            }
        }

        textureUpdateRequired = true;
    }

    void SpaceInvadersVideo::updateTopHalf()
    {
        // Update the top half of the CRT (that is, top half before rotation).
        // We update the various colored regions of the CRT separately.
        // Keep in mind that the CRT is rotated 90 degrees counter-clockwise. Hence, the x coordinate
        // measures the height of an element on the screen.

        unsigned short x = 0, y = 0;

        update(x, y, bottomWhiteRegionHeight, bottomWhiteRegionWidth1, 
            sf::Color::White, sf::Color::Black);

        y += bottomWhiteRegionWidth1;

        update(x, y, bottomWhiteRegionHeight, bottomGreenRegionWidth, 
            sf::Color::Green, sf::Color::Black);

        y += bottomGreenRegionWidth;

        update(x, y, bottomWhiteRegionHeight, crtHeight / 2 - y, 
            sf::Color::Green, sf::Color::Black);

        y = 0;
        x += bottomWhiteRegionHeight;

        updateCommonPart(x, y);
    }

    void SpaceInvadersVideo::updateBottomHalf()
    {
        // Update the various colored regions of the CRT separately.
        // Keep in mind that the CRT is rotated 90 degrees counter-clockwise. Hence, the x coordinate
        // measures the height of an element on the screen.

        unsigned short x = 0, y = crtHeight / 2;

        update(x, y, bottomWhiteRegionHeight, crtHeight / 2, 
            sf::Color::White, sf::Color::Black);

        x += bottomWhiteRegionHeight;

        updateCommonPart(x, y);
    }

    void SpaceInvadersVideo::draw()
    {
        if (textureUpdateRequired)
        {
            texture.update(videoData.get());
            textureUpdateRequired = false;
        }
        
        window.draw(outline);
        window.draw(sprite);
    }

    void SpaceInvadersVideo::updateCommonPart(unsigned short x, unsigned short y)
    {
        update(x, y, greenRegionHeight1, crtHeight / 2, 
            sf::Color::Green, sf::Color::Black);

        x += greenRegionHeight1;

        update(x, y, middleWhiteRegionHeight, crtHeight / 2, 
            sf::Color::White, sf::Color::Black);

        x += middleWhiteRegionHeight;

        update(x, y, redRegionHeight, crtHeight / 2, 
            sf::Color::Red, sf::Color::Black);      

        x += redRegionHeight;
        
        update(x, y, topWhiteRegionHeight, crtHeight / 2, 
            sf::Color::White, sf::Color::Black);  
    }
} // namespace emulator
