#include "consolegui/console.hpp"
#include "consolegui/console_exception.hpp"

namespace emulator
{
    Console::Console()
    {
        inputHandle = GetStdHandle(STD_INPUT_HANDLE);

        if (inputHandle == INVALID_HANDLE_VALUE)
            throw ConsoleException("Failed to get console input handle.");

        outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (outputHandle == INVALID_HANDLE_VALUE)
            throw ConsoleException("Failed to get console output handle.");

        GetConsoleMode(inputHandle, &previousInputMode);
        GetConsoleMode(outputHandle, &previousOutputMode);

        DWORD newMode = previousOutputMode & (~ENABLE_WRAP_AT_EOL_OUTPUT);
        SetConsoleMode(outputHandle, newMode);

        setDirectInputMode(true);
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

    void Console::write(const std::string& text)
    {
        WriteConsoleA(outputHandle, text.c_str(), text.size(), nullptr, nullptr);
    }

    void Console::write(const std::wstring& text)
    {
        WriteConsoleW(outputHandle, text.c_str(), text.size(), nullptr, nullptr);
    }

    void Console::clear()
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
        COORD zero{0, 0};

        GetConsoleScreenBufferInfo(outputHandle, &consoleBufferInfo);
        DWORD numberOfCharacters = consoleBufferInfo.dwSize.X * consoleBufferInfo.dwSize.Y;

        DWORD charactersWritten;
        if (!FillConsoleOutputCharacterA(outputHandle, ' ', numberOfCharacters, zero, &charactersWritten))
        {
            throw ConsoleException("Failed to set output character in Console::clear.");
        }

        if (!FillConsoleOutputAttribute(outputHandle, 0,
            numberOfCharacters, zero, &charactersWritten))
        {
            throw ConsoleException("Failed to set output character attributes in Console::clear.");
        }

        setCursorPosition(0, 0);
    }

    void Console::setCursorVisibility(bool visible)
    {
        CONSOLE_CURSOR_INFO consoleCursorInfo{100, visible};

        if (!SetConsoleCursorInfo(outputHandle, &consoleCursorInfo))
        {
            throw ConsoleException("Failed to set cursor info in Console::setCursorVisibility.");
        }
    }

    void Console::setCursorPosition(const short x, const short y)
    {
        COORD position{x, y};
        SetConsoleCursorPosition(outputHandle, position);
    }

    Position Console::getCursorPosition() const
    {
         CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;

        if (!GetConsoleScreenBufferInfo(outputHandle, &consoleBufferInfo))
        {
            throw ConsoleException("Could not retrieve console buffer info in Console::getScreenSize.");
        }

        return {consoleBufferInfo.dwCursorPosition.X, consoleBufferInfo.dwCursorPosition.Y};       
    }

    WindowPosition Console::getWindowPosition() const
    {
         CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
        
        if (!GetConsoleScreenBufferInfo(outputHandle, &consoleBufferInfo))
        {
            throw ConsoleException("Could not retrieve console buffer info in Console::getScreenSize.");
        }

        return 
            {consoleBufferInfo.srWindow.Left,
             consoleBufferInfo.srWindow.Top, 
             consoleBufferInfo.srWindow.Right,
             consoleBufferInfo.srWindow.Bottom};   
    }

    Size Console::getScreenSize() const
    {
        WindowPosition position = getWindowPosition();
        Size size;
        size.width = position.right - position.left;
        size.height = position.bottom - position.top;
        return size;
    }

    Size Console::getBufferSize() const
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;

        if (!GetConsoleScreenBufferInfo(outputHandle, &consoleBufferInfo))
        {
            throw ConsoleException("Could not retrieve console buffer info in Console::getBufferSize.");
        }

        return {consoleBufferInfo.dwSize.X, consoleBufferInfo.dwSize.Y};
    }

    void Console::setTextColor(const Color foreground, const Color background)
    {
        WORD attributes = static_cast<WORD>(foreground) | (static_cast<WORD>(background) << 4);

        if (!SetConsoleTextAttribute(outputHandle, attributes))
        {
            throw ConsoleException("Could not set text attributes in Console::setColors.");
        }
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
            if (!ReadConsoleA(inputHandle, buffer, 128, &numberOfCharacters, nullptr))
                return false;

            event.line = std::string(buffer, numberOfCharacters);
            return true;
        }
    }
} // namespace emulator
