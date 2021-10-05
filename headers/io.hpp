#pragma once

#include "int_types.hpp"
#include "emulator_exception.hpp"

namespace emulator
{
    /*
        Base class for any class emulating the io ports of a computer system containing an i8080.
    */
    class IO
    {
        public:
            virtual ~IO() {}

            // Called when the processor encounters an IN instruction.
            virtual byte get(byte port) const = 0;

            // Called when the processor encounters an OUT instruction.
            virtual void set(byte port, byte value) = 0;
    };

    /*
        IO port implementation which signals IO is not implemented.
        Throws an EmulatorException when a port is accessed.
    */
    class NotImplementedIO : public IO
    {
        public:
            virtual ~NotImplementedIO() {}

            virtual byte get(byte port) const
            {
                throw EmulatorException("Reading from IO port is not possible. IO operations are not implemented.");
            }

            virtual void set(byte port, byte value)
            {
                throw EmulatorException("Writing to IO port is not possible. IO operations are not implemented.");
            }
    };

    /*
        IO port implementation which does nothing. 
        For testing purposes when IO is not necessary.
        Alwawys returns 0 for any IN instruction, does nothing for any OUT instruction.
    */
    class EmptyIO : public IO
    {
        public:
            virtual ~EmptyIO() {}

            virtual byte get(byte port) const
            {
                return 0;
            }

            virtual void set(byte port, byte value)
            {
                return;
            }
    };
} // namespace emulator
