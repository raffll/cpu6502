#pragma once

#include "i_clock.h"

namespace emulator {

class clock : public i_clock {
public:
    void reset() override;
    void cycle() override;
    size_t get_cycles() override;
    void set_timing(size_t) override;

    ~clock() override = default;

private:
    size_t cycles {};
    size_t timing {};
};

}