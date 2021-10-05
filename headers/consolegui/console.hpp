#pragma once

#include "types.hpp"
#include "screen_buffer.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

namespace console
{
    /*
        A wrapper class around the Win32 console API. 
        
        When created class takes ownership of the console associated to the application. The class
        is not copyable which reflects the fact that an application can only have one console
        associated to it.

        The console class supports two types of input reporting. Either direct input mode, where key pressed
        are reported immediately, or line input mode where events are only generated when a line of text
        is entered into the console and is ended with a carriage return.

        The console class maintains a list of screen buffers. The screen buffers can be written to separately and
        at any time. However, only one buffer can be active and shown to the console screen.
    */
    class Console
    {
        public:
            /*
                Struct used to hold an event reported by the console.
            */
            struct Event
            {
                // Records whether the event was generated while in direct input mode or not.
                // If not in direct input mode the line member will be set otherwise the keyEvent
                // member will hold the information relevant to the event.
                bool isDirectInput;

                KEY_EVENT_RECORD keyEvent;
                std::string line;              
            };

        public:
            explicit Console();
            Console(const Console&) = delete;
            Console(Console&&);
            ~Console();

            // Set the title of the console window.
            void setTitle(const std::string& title);
            void setTitle(const std::wstring& title);

            // Write a string (at the cursor location) to the currently active screen buffer.
            void write(const std::string& text) 
            { 
                getActiveScreenBuffer().write(text); 
            }

            void write(const std::wstring& text) 
            { 
                getActiveScreenBuffer().write(text); 
            }

            // Clears the currently active screen buffer.
            // Puts the cursor back at position (0, 0).
            void clear() 
            { 
                getActiveScreenBuffer().clear(); 
            }

            // Sets whether the cursor in the current screen buffer is visible.
            // Passing true makes the cursor visible.
            void setCursorVisibility(bool visible) 
            { 
                getActiveScreenBuffer().setCursorVisibility(visible); 
            }

            // Sets the cursor position of the currently active screen buffer
            // in the absolute coordinates of the screen buffer (not window coordinates).
            void setCursorPosition(const short x, const short y) 
            { 
                getActiveScreenBuffer().setCursorPosition(x, y);
            }

            // Returns the cursor position of the currently active screen buffer.
            Position getCursorPosition() const 
            {
                return getActiveScreenBuffer().getCursorPosition(); 
            }

            // Returns the coordinates (left, top, right and bottom) of the part of the the currently
            // active screen buffer that is visible.
            WindowPosition getWindowPosition() const
            {
                return getActiveScreenBuffer().getWindowPosition();
            }

            // Sets the coordinates of the part of the currently active screen buffer that is visible.
            void setWindowPosition(const WindowPosition& position)
            {
                getActiveScreenBuffer().setWindowPosition(position);
            }

            // Returns the size (width, height) of the part of the screen buffer that is visible.
            Size getWindowSize() const
            {
                return getActiveScreenBuffer().getWindowSize();
            }

            // Sets the size (width, height) of the part of the screen buffer that is visible.
            void setScreenSize(Size size)
            {
                getActiveScreenBuffer().setScreenSize(size);
            }

            // returns the size of the currently active screen buffer.
            Size getBufferSize() const
            {
                return getActiveScreenBuffer().getBufferSize();
            }

            // Sets the size of the currently active screen buffer.
            void setBufferSize(Size size)
            {
                getActiveScreenBuffer().setBufferSize(size);
            }

            // Sets the foreground and background color of any text that is written to the console
            // after the call to setTextColor.
            void setTextColor(const Color foreground = Color::White, const Color background = Color::Black)
            {
                return getActiveScreenBuffer().setTextColor(foreground, background);
            }

            // Changes the console to either direct input mode or line input mode.
            void setDirectInputMode(bool enabled);
            bool isInDirectInputMode() const { return isInDirectInputMode_; }

            // Polls whether an event is currently waiting in the console's input buffer.
            // If an event is available it is returned into the event parameter and the function returns true
            // else false is returned.
            // A line of input text in line input mode has length of at most 128.
            // The function is non-blocking. 
            bool pollEvent(Event& event);

            // Blocking variant of pollEvent. Waits until an event is waiting in the console's input buffer.
            // Then returns event in the event parameter and the function returns true.
            // The function returns false if an event is available but could not be read succesfully.
            bool waitForEvent(Event& event);

            // Sets the screen buffer that is currently active by index.
            // Throws an ConsoleException if the index is out of range.
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

            // Creates a new screen buffer for the console.
            // Returns the index of the newly created buffer.
            std::size_t createScreenBuffer();

            // Removes a buffer (specified by its index) from the the list of managed screen buffers.
            // Throws a ConsoleException of the index is out of range.
            // Caution: removing a screen buffer from the middle of the list of managed screen buffers
            // will invalidate the indices of the buffers after it.
            void removeScreenBuffer(std::size_t index);
            
        private:
            HANDLE inputHandle, outputHandle;
            DWORD previousInputMode, previousOutputMode;   

            bool isInDirectInputMode_ = true;

            std::vector<ScreenBuffer> buffers;
            std::size_t activeBufferIndex = 0;
    };
}