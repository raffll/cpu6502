#pragma once

namespace emulator {

class i_bus {
public:
    virtual void reset() = 0;
    virtual uint8_t read(uint16_t address) = 0;
    virtual void write(uint16_t address, uint8_t byte) = 0;
    virtual void load_file(std::string filepath) = 0;

    virtual ~i_bus() = default;
};

}