#include "memory.hpp"

#include "emulator_exception.hpp"
#include "defines.hpp"

#include <sstream>
#include <fstream>
#include <cstring>

namespace emulator
{
    Memory::Memory(std::size_t romSize_, std::size_t ramSize_): 
        romSize(romSize_), ramSize(ramSize_), totalSize(romSize_ + ramSize_)
    {
        if (totalSize > maxMemorySize)
        {
            std::stringstream stream;
            stream << "Requested memory size (" << romSize << " + " << ramSize 
                << ") exceeds maximum possible size (" << maxMemorySize << ") in Memory::Memory.";
            throw EmulatorException(stream.str());
        }

        data = std::make_unique<byte[]>(totalSize);
    }

    byte& Memory::operator[] (word address)
    {
        #if EMULATOR_CHECK_BOUNDS
            if (address > totalSize)
                throw EmulatorException(
                    "Memory Address (" + std::to_string(address) + ") out of range in Memory::operator[].");

            if (address < romSize)
                throw EmulatorException(
                    "Memory address (" + std::to_string(address) + ") in ROM can not be set in Memory::operator[].");
        #endif

        return data[address];
    }

    void Memory::set(word address, byte value)
    {
        #if EMULATOR_CHECK_BOUNDS
            if (address > totalSize)
                throw EmulatorException(
                    "Memory address (" + std::to_string(address) + ") out of range in Memory::set.");

            if (address < romSize)
                throw EmulatorException(
                    "Memory address (" + std::to_string(address) + ") in ROM can not be set in Memory::set.");
        #endif

        data[address] = value;
    }

    byte Memory::get(word address) const
    {
        #if EMULATOR_CHECK_BOUNDS
            if (address > totalSize)
                throw EmulatorException(
                    "Memory address (" + std::to_string(address) + ") out of range in Memory::get.");
        #endif

        return data[address];
    }

    word Memory::getWord(word address) const
    {
        #if EMULATOR_CHECK_BOUNDS
            if (address > (totalSize - 1))
                throw EmulatorException(
                    "Memory address (" + std::to_string(address) + ") out of range in Memory::getWord.");
        #endif

        return bytesAsWord(data[address + 1], data[address]);
    }

    void Memory::setWord(word address, word value)
    {
         #if EMULATOR_CHECK_BOUNDS
            if (address > (totalSize - 1))
                throw EmulatorException(
                    "Memory address (" + std::to_string(address) + ") out of range in Memory::setWord.");
        #endif

        wordAsBytePair(value, data[address + 1], data[address]);
    }

    void Memory::clear()
    {
        std::memset(data.get(), 0, totalSize);
    }

    std::size_t Memory::loadMemoryFromFile(const std::string& path, std::size_t offset)
    {
        std::ifstream file(path, std::ios::out | std::ios::binary | std::ios::ate);

        if (!file)
            throw EmulatorException("Unable to open file " + path + " in Memory::loadMemoryFromFile.");

        std::streamsize size = file.tellg();

        if (offset + size > getTotalSize())
        {
            throw EmulatorException(
                "Loading file " + path + " at offset " + std::to_string(offset) +
                " exceeds memory bounds (" + std::to_string(totalSize) + ") in Memory::loadMemoryFromFile.");
        }

        file.seekg(0);

        file.read(reinterpret_cast<char*>(data.get()) + offset, size);
        return offset + size;
    }

    void Memory::loadMemoryFromFiles(const std::vector<std::string> paths, std::size_t offset)
    {
        for (const std::string& path : paths)
            offset = loadMemoryFromFile(path, offset);
    }
} // namespace emulator
