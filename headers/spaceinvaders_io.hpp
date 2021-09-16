#pragma once

#include "io.hpp"

#include <string>
#include <map>

#include <SFML/Window/Keyboard.hpp>

namespace emulator
{
    class SpaceInvadersIO : public IO
    {
        public:
            explicit SpaceInvadersIO();
            virtual ~SpaceInvadersIO();

            virtual byte get(byte port) const override;
            virtual void set(byte port, byte value) override;

            static constexpr bool dip3 = false, dip4 = false, dip5 = false, dip6 = false, dip7 = false;

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

            std::map<std::string, sf::Keyboard::Key> keyMapping;

            word shiftRegister = 0;
            byte offset = 0;
    };
} // namespace emulator
