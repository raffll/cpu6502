#include "clock.h"

namespace emulator {

void clock::reset()
{
    cycles = 0;
}

void clock::cycle()
{
    cycles++;
}

size_t clock::get_cycles()
{
    return cycles;
}

}