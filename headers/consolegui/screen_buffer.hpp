#pragma once

#include "types.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

namespace console
{
    /*
        Wrapper class around a Win32 Console API screen buffer.

        It implements handling cursor position, outputting text, color, window size and buffer size. 
    */
    class ScreenBuffer
    {
        public:
            ScreenBuffer() = delete;
            explicit ScreenBuffer(HANDLE handle, bool ownsHandle = false);
            ScreenBuffer(const ScreenBuffer& other) = delete;
            ScreenBuffer(ScreenBuffer&& other);

            ScreenBuffer& operator=(const ScreenBuffer& other) = delete;
            ScreenBuffer& operator=(ScreenBuffer&& other);

            ~ScreenBuffer();

            // Writes text to the buffer.
            void write(const std::string& text);
            void write(const std::wstring& text);

            // Clears the buffer.
            // Puts the cursor back at position (0, 0).
            void clear();

            // Sets visibility of the cursor.
            // Passing true makes the cursor visible.
            void setCursorVisibility(bool visible);
            
            void setCursorPosition(const short x, const short y);
            Position getCursorPosition() const;

            WindowPosition getWindowPosition() const;
            void setWindowPosition(const WindowPosition& position);

            Size getWindowSize() const;
            void setScreenSize(Size size);

            Size getBufferSize() const;
            void setBufferSize(Size size);

            // Sets the foreground and background color of any text that is written to the buffer
            // after the call to setTextColor.
            void setTextColor(const Color foreground = Color::White, const Color background = Color::Black);

            // Sets the screen buffer to the buffer that is currently displayed in the console window.
            void makeActive();

            static ScreenBuffer create();

        private:
            HANDLE handle;

            // The default console buffer of the application should not be cleaned up after use.
            // Hence we store a flag whether the buffer is owned by the class.
            bool ownsHandle;
    };
} // namespace console
