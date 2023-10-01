#include "bus.h"
#include "types.h"

namespace emulator
{
	bus::bus(size_t size) : memory(size)
	{

	}

	void bus::reset()
	{
		std::fill(memory.begin(), memory.end(), 0x00_uint8_t);
	}

	uint8_t bus::read(uint16_t address)
	{
		return memory.at(address);
	}

	void bus::write(uint16_t address, uint8_t byte)
	{
		memory.at(address) = byte;
	}
}