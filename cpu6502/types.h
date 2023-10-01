#pragma once

#include <cstdint>

constexpr uint8_t operator "" _uint8_t(unsigned long long v)
{
	return static_cast<uint8_t>(v);
}