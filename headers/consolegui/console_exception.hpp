#pragma once

#include <string>

namespace console
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