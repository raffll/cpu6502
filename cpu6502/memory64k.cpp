#include "memory64k.h"

namespace emulator {

void memory64k::reset()
{
    std::fill(memory.begin(), memory.end(), 0x00_u8);
}

uint8_t memory64k::read(uint16_t address)
{
    return memory.at(address);
}

void memory64k::write(uint16_t address, uint8_t byte)
{
    memory.at(address) = byte;
}

void memory64k::load_file(std::string filepath)
{
    std::streampos size;
    std::ifstream file(filepath, std::ifstream::binary);
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        size = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(&memory[0]), size);
        file.close();
    }
}

}