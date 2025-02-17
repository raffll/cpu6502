#include "clock.h"

namespace emulator {

void clock::reset()
{
    cycles = 0;
}

void clock::cycle()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(timing));
    cycles++;
}

size_t clock::get_cycles()
{
    return cycles;
}

void clock::set_timing(size_t t)
{
    timing = t;
}

}