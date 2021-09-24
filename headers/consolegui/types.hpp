#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace console
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
} // namespace emulator
