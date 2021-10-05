#include "spaceinvaders_application.hpp"
#include "diagnostic_application.hpp"

#include "consolegui/console_exception.hpp"
#include "emulator_exception.hpp"

#include <iostream>

#if EMULATOR_LOG_SFML_ERRORS
    #include <SFML/System.hpp>
    #include <fstream>
    #include <chrono>
#endif

void runSpaceInvadersApplication()
{
    emulator::SpaceInvadersApplication app;
    app.run();
}

void runDiagnosticsApplication()
{
    emulator::DiagnosticApplication app;
    app.run();
}

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
        if (argc >= 2)
        {
            std::string argument(argv[1]);
            if (argument == "-d" || argument == "-diagnostic")
            {
                runDiagnosticsApplication();
            }
            else
            {
                runSpaceInvadersApplication();
            }
        }
        else
            runSpaceInvadersApplication();

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