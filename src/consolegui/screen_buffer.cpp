#include "consolegui/screen_buffer.hpp"

#include "consolegui/console_exception.hpp"

namespace console
{
    ScreenBuffer::ScreenBuffer(HANDLE handle_, bool ownsHandle_): 
        handle(handle_), ownsHandle(ownsHandle_)
    {}

    ScreenBuffer::ScreenBuffer(ScreenBuffer&& other):
        handle(other.handle), ownsHandle(handle)
    {
        other.handle = INVALID_HANDLE_VALUE; 
    }

    ScreenBuffer::~ScreenBuffer()
    {
        if (ownsHandle && handle != INVALID_HANDLE_VALUE)
            CloseHandle(handle);
    }

    ScreenBuffer& ScreenBuffer::operator=(ScreenBuffer&& other)
    {
        if (ownsHandle && handle != INVALID_HANDLE_VALUE)
            CloseHandle(handle);

        ownsHandle = other.ownsHandle;
        handle = other.handle;

        other.handle = INVALID_HANDLE_VALUE;

        return *this;
    }

    void ScreenBuffer::write(const std::string& text)
    {
        WriteConsoleA(handle, text.c_str(), text.size(), nullptr, nullptr);
    }

    void ScreenBuffer::write(const std::wstring& text)
    {
        WriteConsoleW(handle, text.c_str(), text.size(), nullptr, nullptr);
    }

    void ScreenBuffer::clear()
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
        COORD zero{0, 0};

        GetConsoleScreenBufferInfo(handle, &consoleBufferInfo);
        DWORD numberOfCharacters = consoleBufferInfo.dwSize.X * consoleBufferInfo.dwSize.Y;

        DWORD numberOfCharactersWritten;
        if (!FillConsoleOutputCharacterA(handle, ' ', numberOfCharacters, zero, &numberOfCharactersWritten))
        {
            throw ConsoleException("Failed to set output character in ScreenBuffer::clear.");
        }

        if (!FillConsoleOutputAttribute(handle, 0,
            numberOfCharacters, zero, &numberOfCharactersWritten))
        {
            throw ConsoleException("Failed to set output character attributes in ScreenBuffer::clear.");
        }

        setCursorPosition(0, 0);
    }

    void ScreenBuffer::setCursorVisibility(bool visible)
    {
        CONSOLE_CURSOR_INFO consoleCursorInfo{100, visible};

        if (!SetConsoleCursorInfo(handle, &consoleCursorInfo))
        {
            throw ConsoleException("Failed to set cursor info in ScreenBuffer::setCursorVisibility.");
        }
    }

    void ScreenBuffer::setCursorPosition(const short x, const short y)
    {
        COORD position{x, y};
        SetConsoleCursorPosition(handle, position);
    }

    Position ScreenBuffer::getCursorPosition() const
    {
         CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;

        if (!GetConsoleScreenBufferInfo(handle, &consoleBufferInfo))
        {
            throw ConsoleException("Could not retrieve console buffer info in ScreenBuffer::getScreenSize.");
        }

        return {consoleBufferInfo.dwCursorPosition.X, consoleBufferInfo.dwCursorPosition.Y};       
    }

    WindowPosition ScreenBuffer::getWindowPosition() const
    {
         CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
        
        if (!GetConsoleScreenBufferInfo(handle, &consoleBufferInfo))
        {
            throw ConsoleException("Could not retrieve console buffer info in ScreenBuffer::getScreenSize.");
        }

        return 
            {consoleBufferInfo.srWindow.Left,
             consoleBufferInfo.srWindow.Top, 
             consoleBufferInfo.srWindow.Right,
             consoleBufferInfo.srWindow.Bottom};   
    }

    void ScreenBuffer::setWindowPosition(const WindowPosition& position)
    {
        SMALL_RECT windowPosition{position.left, position.top, position.right, position.bottom};

        SetConsoleWindowInfo(handle, true, &windowPosition);
    }

    Size ScreenBuffer::getScreenSize() const
    {
        WindowPosition position = getWindowPosition();
        Size size;
        size.width = position.right - position.left;
        size.height = position.bottom - position.top;
        return size;
    }

    void ScreenBuffer::setScreenSize(Size size)
    {
        SMALL_RECT windowPosition{0, 0, size.width, size.height};

        SetConsoleWindowInfo(handle, false, &windowPosition);
    }

    Size ScreenBuffer::getBufferSize() const
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;

        if (!GetConsoleScreenBufferInfo(handle, &consoleBufferInfo))
        {
            throw ConsoleException("Could not retrieve console buffer info in ScreenBuffer::getBufferSize.");
        }

        return {consoleBufferInfo.dwSize.X, consoleBufferInfo.dwSize.Y};
    }

    void ScreenBuffer::setBufferSize(Size size)
    {
        COORD bufferSize{size.width, size.height};

        SetConsoleScreenBufferSize(handle, bufferSize);
    }

    void ScreenBuffer::setTextColor(const Color foreground, const Color background)
    {
        WORD attributes = static_cast<WORD>(foreground) | (static_cast<WORD>(background) << 4);

        if (!SetConsoleTextAttribute(handle, attributes))
        {
            throw ConsoleException("Could not set text attributes in ScreenBuffer::setColors.");
        }
    }

    void ScreenBuffer::makeActive()
    {
        SetConsoleActiveScreenBuffer(handle);
    }

    ScreenBuffer ScreenBuffer::create()
    {
        HANDLE handle = CreateConsoleScreenBuffer(
            GENERIC_WRITE | GENERIC_READ, 
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);

        if (handle == INVALID_HANDLE_VALUE)
            throw ConsoleException("Failed to create new console screen buffer in ScreenBuffer::create.");

        return ScreenBuffer(handle, true);
    }
} // namespace emulator
