#include "memory64k.h"
#include "types.h"

namespace emulator
{
	void memory64k::reset()
	{
		std::fill(memory.begin(), memory.end(), 0x00_u8);
	}

	uint8_t memory64k::read(uint16_t address)
	{
		return memory.at(address);
	}

	void memory64k::write(uint16_t address, uint8_t byte)
	{
		memory.at(address) = byte;
	}
}