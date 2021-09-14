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

            std::map<std::string, sf::Keyboard::Key> keyMapping;
    };
} // namespace emulator
