#include "gtest/gtest.h"
#include "../cpu6502/cpu6502.h"
#include "../cpu6502/clock.h"
#include "../cpu6502/memory64k.h"

namespace emulator
{
	class cpu6502_test : public testing::Test
	{
	public:
		using oc = emulator::cpu6502::opcode;

		emulator::clock clock;
		emulator::memory64k bus;
		emulator::cpu6502 cpu { clock, bus };
		uint16_t address = {};
		bool carry = false;

		void SetUp() override;

		void addressing_IMM(oc opcode, uint8_t data);
		uint16_t addressing_ZPG(oc opcode);
		uint16_t addressing_ZP(oc opcode, uint8_t & offset_reg);
		uint16_t addressing_ABS(oc opcode);
		uint16_t addressing_AB(oc opcode, uint8_t & offset_reg);
		uint16_t addressing_IDX(oc opcode);
		uint16_t addressing_IDY(oc opcode);

		void load_IMM(oc opcode, uint8_t & reg);
		void load_ZPG(oc opcode, uint8_t & reg);
		void load_ZP(oc opcode, uint8_t & reg, uint8_t & offset_reg);
		void load_ABS(oc opcode, uint8_t & reg);
		void load_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg);
		void load_IDX(oc opcode, uint8_t & reg);
		void load_IDY(oc opcode, uint8_t & reg);

		void store_ZPG(oc opcode, uint8_t & reg);
		void store_ZP(oc opcode, uint8_t & reg, uint8_t & offset_reg);
		void store_ABS(oc opcode, uint8_t & reg);
		void store_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg);
		void store_IDX(oc opcode, uint8_t & reg);
		void store_IDY(oc opcode, uint8_t & reg);

		void transfer(oc opcode, uint8_t & src, uint8_t & dst, bool check = true);
		void logic(oc opcode, uint8_t a, uint8_t b);
		void add(oc opcode);
		void substract(oc opcode);
		void compare(oc opcode, uint8_t & reg);
		void increment(oc opcode, uint16_t addr);
	};
}