#pragma once

#include "int_types.hpp"
#include "emulator_exception.hpp"

namespace emulator
{
    class IO
    {
        public:
            virtual ~IO() {}

            virtual byte get(byte port) const = 0;
            virtual void set(byte port, byte value) = 0;
    };

    class EmptyIO : public IO
    {
        public:
            virtual ~EmptyIO() {}

            virtual byte get(byte port) const
            {
                throw EmulatorException("Reading from IO port is not possible. IO operations are not implemented.");
            }

            virtual void set(byte port, byte value)
            {
                throw EmulatorException("Writing to IO port is not possible. IO operations are not implemented.");
            }
    };
} // namespace emulator
