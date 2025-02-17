#pragma once

#include <array>
#include <bit>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <fstream>

constexpr uint8_t operator"" _u8(unsigned long long v)
{
    return static_cast<uint8_t>(v);
}