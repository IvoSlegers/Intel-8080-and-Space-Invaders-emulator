#pragma once

#include <windows.h>
#include <string>
#include <functional>

namespace emulator
{
    struct Size
    {
        short width, height;
    };

    struct Position
    {
        short x, y;
    };

    struct WindowPosition
    {
        short left, top, right, bottom;
    };

    enum class Color : WORD
    {
        Black = 0,
        Red = FOREGROUND_RED,
        Blue = FOREGROUND_BLUE,
        Green = FOREGROUND_GREEN,
        Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
        Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
        Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
        White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        IntenseRed = FOREGROUND_RED | FOREGROUND_INTENSITY,
        IntenseBlue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        IntenseGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        IntenseCyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        IntenseMagenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        IntenseYellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    };

    class Console
    {
        public:
            explicit Console();
            Console(const Console&) = delete;
            Console(Console&&);
            ~Console();

            void setTitle(const std::string& title);
            void setTitle(const std::wstring& title);

            void write(const std::string& text);
            void write(const std::wstring& text);

            void clear();

            void setCursorVisibility(bool visible);
            void setCursorPosition(const short x, const short y);
            Position getCursorPosition() const;

            WindowPosition getWindowPosition() const;
            Size getScreenSize() const;
            Size getBufferSize() const;

            void setTextColor(const Color foreground = Color::White, const Color background = Color::Black);

            void setEchoInput(bool enabled);

            bool pollEvent(KEY_EVENT_RECORD& event);

        private:
            HANDLE inputHandle, outputHandle;
            DWORD previousInputMode, previousOutputMode;   
    };
}