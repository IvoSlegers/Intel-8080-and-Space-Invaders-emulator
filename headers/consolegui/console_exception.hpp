#pragma once

#include <string>
#include <exception>

namespace console
{
    /*
        Exception class that is used report any errors generated when interacting with the Win32 console API.
    */
    class ConsoleException : public std::exception
    {
        public:
            explicit ConsoleException(const std::string& message_): message(message_) {}

            const std::string& getMessage() const
            {
                return message;
            }

            const char* what() const override
            {
                return message.c_str();
            }

        private:
            const std::string message;
    };
}