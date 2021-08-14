#pragma once

#include "int_types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace emulator
{
    class Memory
    {
        public:
            explicit Memory(std::size_t romSize, std::size_t ramSize);

            byte& operator[] (word address);

            void set(word address, byte value);
            byte get(word address) const;

            word getWord(word address) const;
            void setWord(word address, word value);

            std::size_t getRomSize() const
            {
                return romSize;
            }

            std::size_t getRamSize() const
            {
                return ramSize;
            }

            std::size_t getTotalSize() const
            {
                return totalSize;
            }

            std::size_t loadROMFromFile(const std::string& path, std::size_t offset = 0);
            void loadROMFromFiles(const std::vector<std::string> paths, std::size_t offset = 0);

        private:
            std::size_t romSize = 0;
            std::size_t ramSize = 0;
            std::size_t totalSize = 0;

            std::unique_ptr<byte[]> data;

        public:
            static constexpr std::size_t maxMemorySize = (1 << 16);
    };
} // namespace emulator
