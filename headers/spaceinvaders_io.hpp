#pragma once

#include "io.hpp"

#include <string>
#include <map>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>

namespace emulator
{
    /*
        Class that emulates the IO ports founds in the space invaders arcade system.
    */
    class SpaceInvadersIO : public IO
    {
        public:
            explicit SpaceInvadersIO();
            virtual ~SpaceInvadersIO();

            virtual byte get(byte port) const override;
            virtual void set(byte port, byte value) override;

            // The space invaders arcade cabinet had some DIP switches (for the owner of the cabinet) 
            // which regulated some of the game options.
            static constexpr bool dip3 = false, dip4 = false, dip5 = false, dip6 = false, dip7 = false;

            static constexpr std::size_t numberOfSounds = 9;
            static constexpr float soundVolume = 20;

        private:
            byte getPort0() const;
            byte getPort1() const;
            byte getPort2() const;
            byte getPort3() const;

            void setPort2(byte value);
            void setPort3(byte value);
            void setPort4(byte value);
            void setPort5(byte value);
            void setPort6(byte value);

            void handleNonrepeatingSounds(byte value, byte previousValue, std::size_t first, std::size_t number);

            std::map<std::string, sf::Keyboard::Key> keyMapping;

            word shiftRegister = 0;
            byte offset = 0;

            sf::SoundBuffer soundBuffers[numberOfSounds];
            sf::Sound sounds[numberOfSounds];

            byte previousPort3Input = 0;
            byte previousPort5Input = 0;
    };
} // namespace emulator
