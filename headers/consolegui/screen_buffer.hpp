#pragma once

#include "types.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

namespace console
{
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

            void write(const std::string& text);
            void write(const std::wstring& text);

            void clear();

            void setCursorVisibility(bool visible);
            
            void setCursorPosition(const short x, const short y);
            Position getCursorPosition() const;

            WindowPosition getWindowPosition() const;
            void setWindowPosition(const WindowPosition& position);

            Size getScreenSize() const;
            void setScreenSize(Size size);

            Size getBufferSize() const;
            void setBufferSize(Size size);

            void setTextColor(const Color foreground = Color::White, const Color background = Color::Black);

            void makeActive();

            static ScreenBuffer create();
        private:
            HANDLE handle;
            bool ownsHandle;
    };
} // namespace console
