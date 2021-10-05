#pragma once

#include <string>
#include <exception>

namespace emulator
{
    /*
        Exception class used to report any errors in the emulation of the i8080 
        (i.e. invalid opcodes encountered or out of bounds memory access).
    */
    class EmulatorException : public std::exception
    {
        public:
            explicit EmulatorException(const std::string& message_): message(message_) {}

            const std::string& getMessage() const
            {
                return message;
            }

            const char* what() const noexcept override
            {
                return message.c_str();
            }

        private:
            const std::string message;
    };
} // namespace emulator
