#pragma once

#include <array>

#include "i_bus.h"

namespace emulator {

class memory64k : public i_bus {
public:
    void reset() override;
    uint8_t read(uint16_t address) override;
    void write(uint16_t address, uint8_t byte) override;

    ~memory64k() override = default;

private:
    std::array<uint8_t, 64 * 1024> memory = {};
};

}