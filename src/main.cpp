#include "application.hpp"

#include "consolegui/console_exception.hpp"
#include "emulator_exception.hpp"

#include <iostream>

/* TODO
- Modify the opcode_lister.py so that word register instructions show their argument as BC, DE etc
- get a stepwise simulation running
*/

int main(void)
{
    try
    {
        emulator::Application app;
        app.run();
    }
    catch (const emulator::ConsoleException& exception)
    {
        std::cerr << "Console exception: " << exception.getMessage() << '\n';
        std::cin.get();
    }
    catch (const emulator::EmulatorException& exception)
    {
        std::cerr << "Emulator exception: " << exception.getMessage() << '\n';
        std::cin.get();     
    }

    return 0;
}