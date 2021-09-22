#pragma once

namespace emulator
{
    class Application
    {
        public:
            virtual ~Application() {}

            virtual void run() = 0;
    };
} // namespace emulator
