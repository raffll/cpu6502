#pragma once

#include <cstdint>
#include <fstream>

constexpr uint8_t operator"" _u8(unsigned long long v)
{
    return static_cast<uint8_t>(v);
}