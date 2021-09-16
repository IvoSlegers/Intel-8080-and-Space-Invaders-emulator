#include "application.hpp"

#include "consolegui/console_exception.hpp"
#include "emulator_exception.hpp"
#include "defines.hpp"

#include <iostream>

// TODO
// - implement screen drawing interrupt system
// - implement sound
// - implement port 6 output = reset
// - add a parameter scaling factor to video component
// - Let executeInstructionCycle + issueInterrupt return the number of machine cycles
// - Wrap all Space Invaders specific stuff into a SpaceInvadersCabinet class.
// - Implement breakpoints system
// Possible interesting breakpoint 2778
// - Implement draw dialog prompt in draw function of consoleUI.
// - Using a set for breakpoints is potentially slow, replace by large bool array?

#if EMULATOR_LOG_SFML_ERRORS
    #include <SFML/System.hpp>
    #include <fstream>
    #include <chrono>
#endif


int main(void)
{
    #if EMULATOR_LOG_SFML_ERRORS
        std::ofstream logFile("error_log.txt", std::ios_base::app);
        logFile << "--------------------------------------------------------\n";
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        logFile << "Log file of run at " <<  std::ctime(&now) << '\n';
        sf::err().rdbuf(logFile.rdbuf());
    #endif

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
    catch (const std::exception& exception)
    {
        std::cerr << "Std exception: " << exception.what() << '\n';
        std::cin.get();
    }

    return 0;
}