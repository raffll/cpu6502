#pragma once

namespace emulator {

class i_clock {
public:
    virtual void reset() = 0;
    virtual void cycle() = 0;
    virtual size_t get_cycles() = 0;

    virtual ~i_clock() = default;
};

}
