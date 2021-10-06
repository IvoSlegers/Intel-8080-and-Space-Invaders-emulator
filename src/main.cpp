#include "spaceinvaders_application.hpp"
#include "diagnostic_application.hpp"

#include "consolegui/console_exception.hpp"
#include "emulator_exception.hpp"

#include <iostream>

using emulator::Application;
using emulator::DiagnosticApplication;
using emulator::SpaceInvadersApplication;

void runApplication(Application& app);

#if EMULATOR_LOG_SFML_ERRORS
    #include <SFML/System.hpp>
    #include <fstream>
    #include <chrono>

    void directSFMLErrorStreamToFile();
#endif

int main(int argc, const char* argv[])
{
    #if EMULATOR_LOG_SFML_ERRORS
        directSFMLErrorStreamToFile();
    #endif

    bool runDiagnostic = false;
    if (argc >= 2)
    {
        std::string argument(argv[1]);
        if (argument == "-d" || argument == "-diagnostic")
            runDiagnostic = true;
    }

    if (runDiagnostic)
    {
        DiagnosticApplication application;
        runApplication(application);
    }
    else
    {
        SpaceInvadersApplication application;
        runApplication(application);
    }

    return EXIT_SUCCESS;  
}

void runApplication(Application& application)
{
    try
    {
        application.run();
    }
    catch (const console::ConsoleException& exception)
    {
        std::cerr << "Console exception encountered: " << exception.what() << '\n';
        std::cin.get();
    }
    catch (const emulator::EmulatorException& exception)
    {
        std::cerr << "Emulator exception encountered: " << exception.what() << '\n';
        std::cin.get();     
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Exception encountered: " << exception.what() << '\n';
        std::cin.get();
    }
}

#if EMULATOR_LOG_SFML_ERRORS
    void directSFMLErrorStreamToFile();
    {
        std::ofstream logFile("error_log.txt", std::ios_base::app);

        logFile << "--------------------------------------------------------\n";
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        logFile << "Log file of run at " <<  std::ctime(&now) << '\n';

        sf::err().rdbuf(logFile.rdbuf());
    }
#endif