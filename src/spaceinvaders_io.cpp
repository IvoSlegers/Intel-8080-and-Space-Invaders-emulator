#include "spaceinvaders_io.hpp"

#include <string>

namespace emulator
{
    namespace
    {
        enum Sounds
        {
            Ufo = 0,
            Shot,
            Flash,
            InvaderDie,
            FleetMovement1,
            FleetMovement2,
            FleetMovement3,
            FleetMovement4,
            UfoHit     
        };

        static const byte soundMasks[SpaceInvadersIO::numberOfSounds] = 
        {
            0x01,
            0x02,
            0x04,
            0x08,
            0x01,
            0x02,
            0x04,
            0x08,
            0x10 
        };

        static const std::string soundFileNames[SpaceInvadersIO::numberOfSounds] = 
        {
            "sounds/ufo.wav",
            "sounds/shot.wav",
            "sounds/flash.wav",
            "sounds/invader_die.wav",
            "sounds/fleet_movement_1.wav",
            "sounds/fleet_movement_2.wav",
            "sounds/fleet_movement_3.wav",
            "sounds/fleet_movement_4.wav",
            "sounds/ufo_hit.wav"
        };
    }

    SpaceInvadersIO::SpaceInvadersIO()
    {
        keyMapping["Fire"] = sf::Keyboard::Space;
        keyMapping["Left"] = sf::Keyboard::Left;
        keyMapping["Right"] = sf::Keyboard::Right;

        keyMapping["Coin Inserted"] = sf::Keyboard::C;
        keyMapping["2 Players Start"] = sf::Keyboard::Num2;
        keyMapping["1 Player Start"] = sf::Keyboard::Num1;

        keyMapping["1 Player Fire"] = sf::Keyboard::Space;
        keyMapping["1 Player Left"] = sf::Keyboard::Left;
        keyMapping["1 Player Right"] = sf::Keyboard::Right;

        keyMapping["2 Player Fire"] = sf::Keyboard::LControl;
        keyMapping["2 Player Left"] = sf::Keyboard::A;
        keyMapping["2 Player Right"] = sf::Keyboard::D;

        for (std::size_t i = 0; i < SpaceInvadersIO::numberOfSounds; ++i)
        {
            sounds[i].setVolume(soundVolume);
            sounds[i].setLoop(false);

            if (soundBuffers[i].loadFromFile(soundFileNames[i]))
                sounds[i].setBuffer(soundBuffers[i]);
        }
    }

    SpaceInvadersIO::~SpaceInvadersIO()
    {}

    byte SpaceInvadersIO::get(byte port) const
    {
        switch (port)
        {
            case 0:
                return getPort0();
            
            case 1:
                return getPort1();

            case 2:
                return getPort2();

            case 3:
                return getPort3();

            default:
                throw EmulatorException("Reading from IO port " + std::to_string(port) + " not implemented in SpaceInvadersIO::get.");
        }       
    }

    void SpaceInvadersIO::set(byte port, byte value)
    {
        switch (port)
        {
            case 2:
                setPort2(value);
                return;
                
            case 3:
                setPort3(value);
                return;

            case 4:
                setPort4(value);
                return;

            case 5:
                setPort5(value);
                return;

            case 6:
                setPort6(value);
                return;

            default:
                throw EmulatorException("Writing to IO port " + std::to_string(port) + " not implemented in SpaceInvadersIO::set.");
        }
    }

    byte SpaceInvadersIO::getPort0() const
    {
        /*
        Not actually used by space invaders code.
        Port 0
        bit 0 DIP4 (Seems to be self-test-request read at power up)
        bit 1 Always 1
        bit 2 Always 1
        bit 3 Always 1
        bit 4 Fire
        bit 5 Left
        bit 6 Right
        bit 7 ?
        */

       bool fire = sf::Keyboard::isKeyPressed(keyMapping.at("Fire"));
       bool left = sf::Keyboard::isKeyPressed(keyMapping.at("Left"));
       bool right = sf::Keyboard::isKeyPressed(keyMapping.at("Right"));

        return 0b0000'1110 | (fire << 4) | (left << 5) | (right << 6);
    }

    byte SpaceInvadersIO::getPort1() const
    {
        /*
        Port 1
        bit 0 = CREDIT (1 if deposit)
        bit 1 = 2P start (1 if pressed)
        bit 2 = 1P start (1 if pressed)
        bit 3 = Always 1
        bit 4 = 1P shot (1 if pressed)
        bit 5 = 1P left (1 if pressed)
        bit 6 = 1P right (1 if pressed)
        bit 7 = Not connected
        */   

        bool coinInserted = sf::Keyboard::isKeyPressed(keyMapping.at("Coin Inserted"));
        bool twoPlayerStart = sf::Keyboard::isKeyPressed(keyMapping.at("2 Players Start"));
        bool onePlayerStart = sf::Keyboard::isKeyPressed(keyMapping.at("1 Player Start"));

        bool onePlayerFire = sf::Keyboard::isKeyPressed(keyMapping.at("1 Player Fire"));
        bool onePlayerLeft = sf::Keyboard::isKeyPressed(keyMapping.at("1 Player Left"));
        bool onePlayerRight = sf::Keyboard::isKeyPressed(keyMapping.at("1 Player Right"));

        return 0b0000'1000 | coinInserted | (twoPlayerStart << 1) | (onePlayerStart << 2) |
            (onePlayerFire << 4) | (onePlayerLeft << 5) | (onePlayerRight << 6);
    }

    byte SpaceInvadersIO::getPort2() const
    {
        /*
        Port 2
        bit 0 = DIP3 00 = 3 ships  10 = 5 ships
        bit 1 = DIP5 01 = 4 ships  11 = 6 ships
        bit 2 = Tilt
        bit 3 = DIP6 0 = extra ship at 1500, 1 = extra ship at 1000
        bit 4 = P2 shot (1 if pressed)
        bit 5 = P2 left (1 if pressed)
        bit 6 = P2 right (1 if pressed)
        bit 7 = DIP7 Coin info displayed in demo screen 0=ON
        */

        bool twoPlayerFire = sf::Keyboard::isKeyPressed(keyMapping.at("2 Player Fire"));
        bool twoPlayerLeft = sf::Keyboard::isKeyPressed(keyMapping.at("2 Player Left"));
        bool twoPlayerRight = sf::Keyboard::isKeyPressed(keyMapping.at("2 Player Right"));

        return 0b0000'0000 | dip3 | (dip5 << 1) | (dip6 << 3) | (twoPlayerFire << 4) |
            (twoPlayerLeft << 5) | (twoPlayerRight << 6) | (dip7 << 7);
    }

    byte SpaceInvadersIO::getPort3() const
    {
        return (shiftRegister >> (8 - offset));
    }

    void SpaceInvadersIO::setPort2(byte value)
    {
        offset = value & 0b0000'0111;
    }

    void SpaceInvadersIO::setPort3(byte value)
    {
        if ((value & soundMasks[Sounds::Ufo]) != 0 && 
            sounds[Sounds::Ufo].getStatus() != sf::Sound::Playing)
        {
            sounds[Sounds::Ufo].play();
        }

        handleNonrepeatingSounds(value, previousPort3Input, 1, 3);

        previousPort3Input = value;

        return;
    }

    void SpaceInvadersIO::setPort4(byte value)
    {
        shiftRegister = (value << 8) | (shiftRegister >> 8);
    }

    void SpaceInvadersIO::setPort5(byte value)
    {
        handleNonrepeatingSounds(value, previousPort5Input, 4, 5);
        previousPort5Input = value;
        return;
    }

    void SpaceInvadersIO::setPort6(byte value)
    {
        /*
        Port 6 is used for reseting the game. Currently not implemented.
        */

        return;
    }    

    void SpaceInvadersIO::handleNonrepeatingSounds(byte value, byte previousValue, std::size_t first, std::size_t number)
    {
        for (std::size_t i = first; i < first + number; ++i)
        {
            if ((value & soundMasks[i]) != 0 && 
                (previousValue & soundMasks[i]) == 0 && 
                sounds[i].getStatus() != sf::Sound::Playing)
            {
                sounds[i].play();
            }
        }
    }
} // namespace emulator
