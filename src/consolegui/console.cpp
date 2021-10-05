#include "consolegui/console.hpp"

#include "consolegui/console_exception.hpp"

namespace console
{
    Console::Console()
    {
        inputHandle = GetStdHandle(STD_INPUT_HANDLE);

        if (inputHandle == INVALID_HANDLE_VALUE)
            throw ConsoleException("Failed to get console input handle in Console::Console.");

        outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (outputHandle == INVALID_HANDLE_VALUE)
            throw ConsoleException("Failed to get console output handle in Console::Console.");

        GetConsoleMode(inputHandle, &previousInputMode);
        GetConsoleMode(outputHandle, &previousOutputMode);

        // Disable word wrap
        DWORD newMode = previousOutputMode & (~ENABLE_WRAP_AT_EOL_OUTPUT);
        SetConsoleMode(outputHandle, newMode);

        setDirectInputMode(true);

        buffers.emplace_back(outputHandle);
    }

    Console::Console(Console&& other): inputHandle(other.inputHandle), outputHandle(other.outputHandle),
        previousInputMode(other.previousInputMode), previousOutputMode(other.previousOutputMode) 
    {
        other.inputHandle = other.outputHandle = INVALID_HANDLE_VALUE;
    }

    Console::~Console()
    {
        if (inputHandle != INVALID_HANDLE_VALUE)
            SetConsoleMode(inputHandle, previousInputMode);

        if (outputHandle != INVALID_HANDLE_VALUE)
            SetConsoleMode(outputHandle, previousOutputMode);
    }

    void Console::setTitle(const std::string& title)
    {
        SetConsoleTitleA(title.c_str());
    }

    void Console::setTitle(const std::wstring& title)
    {
        SetConsoleTitleW(title.c_str());
    }

    void Console::setDirectInputMode(bool enabled)
    {
        isInDirectInputMode_ = enabled;

        DWORD mode;
        GetConsoleMode(inputHandle, &mode);

        if (enabled)
            mode &= (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT);
        else
            mode |= ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;            

        SetConsoleMode(inputHandle, mode);
    }

    bool Console::pollEvent(Event& event)
    {
        // The functions ReadConsoleInput and ReadConsoleA are blocking so we only 
        // call these if an event is in the input buffer.
        DWORD numberOfEvents;
        if (!GetNumberOfConsoleInputEvents(inputHandle, &numberOfEvents))
            return false;

        if (numberOfEvents == 0)
            return false;

        event.isDirectInput = isInDirectInputMode_;
        if (isInDirectInputMode_)
        {
            INPUT_RECORD record;
            DWORD numberOfEventsRead;

            // This is a blocking function.
            if (!ReadConsoleInput(inputHandle, &record, 1, &numberOfEventsRead))
                return false;

            if (record.EventType != KEY_EVENT)
                return false;

            event.keyEvent = record.Event.KeyEvent;
            return true;
        }
        else
        {
            char buffer[128];
            DWORD numberOfCharacters;

            // This is a blocking function.
            if (!ReadConsoleA(inputHandle, buffer, 128, &numberOfCharacters, nullptr))
                return false;

            event.line = std::string(buffer, numberOfCharacters);
            return true;
        }
    }

    bool Console::waitForEvent(Event& event)
    {
        event.isDirectInput = isInDirectInputMode_;
        if (isInDirectInputMode_)
        {
            INPUT_RECORD record;
            DWORD numberOfEventsRead;

            while (true)
            {
                // This is a blocking function.
                if (!ReadConsoleInput(inputHandle, &record, 1, &numberOfEventsRead))
                    return false;

                // If the event read from the input buffer is not a keyboard event we try again.
                if (record.EventType != KEY_EVENT)
                    continue;

                event.keyEvent = record.Event.KeyEvent;
                    return true;
            }
        }
        else
        {
            char buffer[128];
            DWORD numberOfCharacters;

            // This is a blocking function.
            if (!ReadConsoleA(inputHandle, buffer, 128, &numberOfCharacters, nullptr))
                return false;

            event.line = std::string(buffer, numberOfCharacters);
            return true;
        }
    }

    void Console::setActiveScreenBuffer(std::size_t index)
    {
        if (index >= buffers.size())
            throw ConsoleException("Screen buffer index out of range in Console::setActiveScreenBuffer.");

        activeBufferIndex = index;
        buffers[activeBufferIndex].makeActive();
    }

    void Console::restoreDefaultScreenBuffer()
    {
        setActiveScreenBuffer(0);
    }

    ScreenBuffer& Console::getScreenBuffer(std::size_t index)
    {
        if (index >= buffers.size())
            throw ConsoleException("Screen buffer index out of range in Console::getScreenBuffer.");

        return buffers[index];
    }

    std::size_t Console::createScreenBuffer()
    {
        ScreenBuffer buffer = ScreenBuffer::create();

        buffers.push_back(std::move(buffer));
        return buffers.size() - 1;
    }

    void Console::removeScreenBuffer(std::size_t index)
    {
        if (index >= buffers.size())
            throw ConsoleException("Screen buffer index out of range in Console::removeScreenBuffer.");

        if (index == 0)
            throw ConsoleException("Default screen buffer at index 0 can not be removed in Console::removeScreenBuffer.");

        buffers.erase(buffers.begin() + index);
    }
} // namespace console
