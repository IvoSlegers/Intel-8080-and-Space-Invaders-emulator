#include "application.hpp"

#include "to_hex_string.hpp"

namespace emulator
{
    Application::Application(): memory(0x2000, 0x2000), io(), cpu(memory, io),
        consoleUI(cpu, memory)
    {
        memory.loadROMFromFile("invaders.rom");
    }

    void Application::run()
    {
        consoleUI.draw();
        ui.run();
        
        // auto callback = [this] (const KEY_EVENT_RECORD& event) 
        // {
        //     return this->onKeyEvent(event);
        // };

        // console.run(callback);
    }

    bool Application::onKeyEvent(const KEY_EVENT_RECORD& event)
    {
        if(event.bKeyDown && event.wVirtualKeyCode == VK_SPACE)
        {
            cpu.executeInstructionCycle();
            //draw();
        }

        return (event.uChar.AsciiChar != 'q');
    }    
} // namespace emulator
