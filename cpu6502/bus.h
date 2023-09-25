#pragma once

#include <vector>
#include <cassert>

#include "i_bus.h"

namespace emulator
{
	class bus : public i_bus
	{
	public:
		bus(size_t size = 64 * 1024);

		void reset() override;
		uint8_t read(uint16_t address) override;
		void write(uint16_t address, uint8_t byte) override;

	private:
		// 0x0000-0x00FF zero page
		// 0x0100-0x01FF stack
		// 0x0200-0xFFF9 program
		// 0xFFFA-0xFFFF special
		std::vector<uint8_t> memory;
	};
}