#pragma once

#include <string>

namespace emulator
{
    class EmulatorException
    {
        public:
            explicit EmulatorException(const std::string& message_): message(message_) {}

            const std::string& getMessage() const
            {
                return message;
            }

        private:
            const std::string message;
    };
} // namespace emulator
