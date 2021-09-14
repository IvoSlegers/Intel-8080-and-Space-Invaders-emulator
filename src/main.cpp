#include "application.hpp"

#include "consolegui/console_exception.hpp"
#include "emulator_exception.hpp"

#include <iostream>

/* TODO
- draw video memory
*/

#include <SFML/System.hpp>
#include <fstream>
#include <chrono>

int main(void)
{
    std::ofstream logFile("error_log.txt", std::ios_base::app);
    logFile << "--------------------------------------------------------\n";
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logFile << "Log file of run at " <<  std::ctime(&now) << '\n';
    sf::err().rdbuf(logFile.rdbuf());

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