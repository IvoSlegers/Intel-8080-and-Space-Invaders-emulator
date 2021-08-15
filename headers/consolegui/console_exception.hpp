#pragma once

#include <string>

namespace emulator
{
    class ConsoleException
    {
        public:
            explicit ConsoleException(const std::string& message_): message(message_) {}

            const std::string& getMessage() const
            {
                return message;
            }

        private:
            const std::string message;
    };
}