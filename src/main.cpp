#include "spaceinvaders_application.hpp"
#include "diagnostic_application.hpp"

#include "consolegui/console_exception.hpp"
#include "emulator_exception.hpp"
#include "defines.hpp"

#include <iostream>
#include <sstream>

// TODO
// - implement screen drawing interrupt system
// - implement sound
// - implement port 6 output = reset
// - add a parameter scaling factor to video component
// - Wrap all Space Invaders specific stuff into a SpaceInvadersCabinet class.
// - Implement breakpoints system
// Possible interesting breakpoint 2778
// - Using a set for breakpoints is potentially slow, replace by large bool array?
// - Remove all the 'executed machine cycles += 5' with a table lookup
// - Make the switch statement nicer (see supersazu's emulator)
// - Implement a buffer system for the console so we can write a separate log messages page
// - Or... pipe any output from diagnostics into a file?
// - Implement a run until end function for cpu diagnostics application
// - implement move, view, breakpoint commands
// - Make classes that need to be non-copyable
// - Look at unique_ptr to see how a move-only class should be implemented.
// - Properly implement a executed instructions log (move run until halt & logging into consoleUI)
// - Implement cyc checking?
// - re-enable check bounds

// - GOAL: make emulator compliant

#if EMULATOR_LOG_SFML_ERRORS
    #include <SFML/System.hpp>
    #include <fstream>
    #include <chrono>
#endif

int main(int argc, const char* argv[])
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
        //emulator::SpaceInvadersApplication app;
        emulator::DiagnosticApplication app;
        app.run();
    }
    catch (const console::ConsoleException& exception)
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