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

		~bus() override {}

	private:
		std::vector<uint8_t> memory;
	};
}