#pragma once

#include "types.hpp"
#include "screen_buffer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

namespace console
{
    class Console
    {
        public:
            struct Event
            {
                bool isDirectInput;

                KEY_EVENT_RECORD keyEvent;
                std::string line;              
            };

        public:
            explicit Console();
            Console(const Console&) = delete;
            Console(Console&&);
            ~Console();

            void setTitle(const std::string& title);
            void setTitle(const std::wstring& title);

            void write(const std::string& text) 
            { 
                getActiveScreenBuffer().write(text); 
            }

            void write(const std::wstring& text) 
            { 
                getActiveScreenBuffer().write(text); 
            }

            void clear() 
            { 
                getActiveScreenBuffer().clear(); 
            }

            void setCursorVisibility(bool visible) 
            { 
                getActiveScreenBuffer().setCursorVisibility(visible); 
            }

            void setCursorPosition(const short x, const short y) 
            { 
                getActiveScreenBuffer().setCursorPosition(x, y);
            }

            Position getCursorPosition() const 
            {
                return getActiveScreenBuffer().getCursorPosition(); 
            }

            WindowPosition getWindowPosition() const
            {
                return getActiveScreenBuffer().getWindowPosition();
            }

            void setWindowPosition(const WindowPosition& position)
            {
                getActiveScreenBuffer().setWindowPosition(position);
            }

            Size getScreenSize() const
            {
                return getActiveScreenBuffer().getScreenSize();
            }

            void setScreenSize(Size size)
            {
                getActiveScreenBuffer().setScreenSize(size);
            }

            Size getBufferSize() const
            {
                return getActiveScreenBuffer().getBufferSize();
            }

            void setBufferSize(Size size)
            {
                getActiveScreenBuffer().setBufferSize(size);
            }

            void setTextColor(const Color foreground = Color::White, const Color background = Color::Black)
            {
                return getActiveScreenBuffer().setTextColor(foreground, background);
            }

            void setDirectInputMode(bool enabled);
            bool isInDirectInputMode() const { return isInDirectInputMode_; }

            bool pollEvent(Event& event);
            bool waitForEvent(Event& event);

            void setActiveScreenBuffer(std::size_t index);
            void restoreDefaultScreenBuffer();

            std::size_t getActiveScreenBufferIndex() const 
            {
                return activeBufferIndex;
            }

            ScreenBuffer& getActiveScreenBuffer()
            {
                return buffers[activeBufferIndex];
            }

            const ScreenBuffer& getActiveScreenBuffer() const
            {
                return buffers[activeBufferIndex];
            }

            ScreenBuffer& getDefaultScreenBuffer()
            {
                return buffers[0];
            }

            ScreenBuffer& getScreenBuffer(std::size_t index);
            
            std::size_t getNumberOfScreenBuffers() const
            {
                return buffers.size();
            }

            std::size_t createScreenBuffer();
            void removeScreenBuffer(std::size_t index);
            
        private:
            HANDLE inputHandle, outputHandle;
            DWORD previousInputMode, previousOutputMode;   

            bool isInDirectInputMode_ = true;

            std::vector<ScreenBuffer> buffers;
            std::size_t activeBufferIndex = 0;
    };
}