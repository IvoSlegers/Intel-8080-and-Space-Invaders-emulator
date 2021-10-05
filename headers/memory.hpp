#pragma once

#include "int_types.hpp"

#include <memory>
#include <string>
#include <vector>

namespace emulator
{
    /*
        Class that implements the emulation of the memory modules in a computer system containing an i8080.
    */
    class Memory
    {
        public:
            // Allocates romSize + ramSize memory. The block 0 through (romSize - 1) is read-only.
            // The block romSize through (romSize + ramSize - 1) is read and write memory.
            // Throws an EmulatorException if romSize + ramSize > 0xFFFF (the maximal size that 
            // can be indexed by a word).
            explicit Memory(std::size_t romSize, std::size_t ramSize);

            byte& operator[] (word address);

            void set(word address, byte value);
            byte get(word address) const;

            // Helper functions for indexing two consecutive byes as a word.
            // Caution: the intel 8080 is a little endian system. Hence
            // a word in memory is stored as ... <low byte> <high byte> ...
            // Always use these functions to correctly read words stored in little endian fashion.
            word getWord(word address) const;
            void setWord(word address, word value);

            // Fills the memory array with zeroes.
            void clear();

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

            // Loads the contents of the given file into memory at a given offset.
            // Throws an EmulatorException if the given file could not be opened.
            // Throws an EmulatorException if the file does not fit in memory at the given offset.
            // Returns the index of the first byte after the contents of the loaded file.
            std::size_t loadMemoryFromFile(const std::string& path, std::size_t offset = 0);

            // Loads the contents of multiple files sequentially into memory at a given offset.
            void loadMemoryFromFiles(const std::vector<std::string> paths, std::size_t offset = 0);

        private:
            std::size_t romSize = 0;
            std::size_t ramSize = 0;
            std::size_t totalSize = 0;

            std::unique_ptr<byte[]> data;

        public:
            static constexpr std::size_t maxMemorySize = (1 << 16);
    };
} // namespace emulator
