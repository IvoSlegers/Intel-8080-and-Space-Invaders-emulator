#pragma once

namespace emulator
{
    /*
        Base class to any application class.
    */
    class Application
    {
        public:
            virtual ~Application() {}

            virtual void run() = 0;
    };
} // namespace emulator
