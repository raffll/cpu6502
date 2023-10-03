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

		void SetUp() override
		{
			clock.reset();
			bus.reset();
			cpu.reset();
			address = 0x0200;
		}

		void addressing_IMM(oc opcode, uint8_t data)
		{
			bus.write(address++, opcode);
			bus.write(address++, data);
		}

		void addressing_ZPG(oc opcode, uint8_t lo)
		{
			bus.write(address++, opcode);
			bus.write(address++, lo);
		}

		void addressing_ZP(oc opcode, uint8_t & offset_reg, uint8_t lo)
		{
			offset_reg = 0x0F;
			bus.write(address++, opcode);
			bus.write(address++, lo);
		}

		void addressing_ABS(oc opcode, uint8_t lo, uint8_t hi)
		{
			bus.write(address++, opcode);
			bus.write(address++, lo);
			bus.write(address++, hi);
		}

		void addressing_AB(oc opcode, uint8_t & offset_reg, uint8_t lo, uint8_t hi, bool carry = false)
		{
			if (carry)
				offset_reg = 0xFF;
			else
				offset_reg = 0x0F;

			bus.write(address++, opcode);
			bus.write(address++, lo);
			bus.write(address++, hi);
		}

		void addressing_IDX(oc opcode, uint8_t lo, uint8_t hi)
		{
			cpu.X = 0x0F;
			bus.write(address++, opcode);
			bus.write(address++, 0x20);
			bus.write(0x002F, lo);
			bus.write(0x0030, hi);
		}

		void addressing_IDY(oc opcode, uint8_t lo, uint8_t hi, bool carry = false)
		{
			if (carry)
				cpu.Y = 0xFF;
			else
				cpu.Y = 0x0F;

			bus.write(address++, opcode);
			bus.write(address++, 0x20);
			bus.write(0x0020, lo);
			bus.write(0x0021, hi);
		}

		void load_IMM(oc opcode, uint8_t & reg);
		void load_ZPG(oc opcode, uint8_t & reg);
		void load_ZP(oc opcode, uint8_t & reg, uint8_t & offset_reg);
		void load_ABS(oc opcode, uint8_t & reg);
		void load_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg, bool carry = false);
		void load_IDX(oc opcode, uint8_t & reg);
		void load_IDY(oc opcode, uint8_t & reg, bool carry = false);

		void store_ZPG(oc opcode, uint8_t & reg);
		void store_ZP(oc opcode, uint8_t & reg, uint8_t & offset_reg);
		void store_ABS(oc opcode, uint8_t & reg);
		void store_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg, bool carry = false);
		void store_IDX(oc opcode, uint8_t & reg);
		void store_IDY(oc opcode, uint8_t & reg, bool carry = false);

		void transfer(oc opcode, uint8_t & src, uint8_t & dst, bool check = true);
		void logic(oc opcode, uint8_t a, uint8_t b);
		void add(oc opcode);
		void substract(oc opcode);
		void compare(oc opcode, uint8_t & reg);
	};

	void cpu6502_test::load_IMM(oc opcode, uint8_t & reg)
	{
		addressing_IMM(opcode, 0x0A);
		cpu.execute();

		EXPECT_EQ(reg, 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

		addressing_IMM(opcode, 0x00);
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(opcode, 0x80);
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, LDA_IMM)
	{
		load_IMM(oc::LDA_IMM, cpu.A);
	}

	TEST_F(cpu6502_test, LDX_IMM)
	{
		load_IMM(oc::LDX_IMM, cpu.X);
	}

	TEST_F(cpu6502_test, LDY_IMM)
	{
		load_IMM(oc::LDY_IMM, cpu.Y);
	}

	void cpu6502_test::load_ZPG(oc opcode, uint8_t & reg)
	{
		addressing_ZPG(opcode, 0x0A);
		bus.write(0x000A, 0x0B);
		cpu.execute();

		EXPECT_EQ(reg, 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, LDA_ZPG)
	{
		load_ZPG(oc::LDA_ZPG, cpu.A);
	}

	TEST_F(cpu6502_test, LDX_ZPG)
	{
		load_ZPG(oc::LDX_ZPG, cpu.X);
	}

	TEST_F(cpu6502_test, LDY_ZPG)
	{
		load_ZPG(oc::LDY_ZPG, cpu.Y);
	}

	void cpu6502_test::load_ZP(oc opcode, uint8_t & reg, uint8_t & offset_reg)
	{
		addressing_ZP(opcode, offset_reg, 0x80);
		bus.write(0x0080 + offset_reg, 0x0C);
		cpu.execute();

		EXPECT_EQ(reg, 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, LDA_ZPX)
	{
		load_ZP(oc::LDA_ZPX, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, LDX_ZPY)
	{
		load_ZP(oc::LDX_ZPY, cpu.X, cpu.Y);
	}

	TEST_F(cpu6502_test, LDY_ZPX)
	{
		load_ZP(oc::LDY_ZPX, cpu.Y, cpu.X);
	}

	void cpu6502_test::load_ABS(oc opcode, uint8_t & reg)
	{
		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x0D);
		cpu.execute();

		EXPECT_EQ(reg, 0x0D);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, LDA_ABS)
	{
		load_ABS(oc::LDA_ABS, cpu.A);
	}

	TEST_F(cpu6502_test, LDX_ABS)
	{
		load_ABS(oc::LDX_ABS, cpu.X);
	}

	TEST_F(cpu6502_test, LDY_ABS)
	{
		load_ABS(oc::LDY_ABS, cpu.Y);
	}

	void cpu6502_test::load_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg, bool carry)
	{
		addressing_AB(opcode, offset_reg, 0x80, 0x40, carry);
		bus.write(0x4080 + offset_reg, 0x0E);
		cpu.execute();

		EXPECT_EQ(reg, 0x0E);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
	}

	TEST_F(cpu6502_test, LDA_ABX)
	{
		load_AB(oc::LDA_ABX, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, LDA_ABX_C)
	{
		load_AB(oc::LDA_ABX, cpu.A, cpu.X, true);
	}

	TEST_F(cpu6502_test, LDA_ABY)
	{
		load_AB(oc::LDA_ABY, cpu.A, cpu.Y);
	}

	TEST_F(cpu6502_test, LDA_ABY_C)
	{
		load_AB(oc::LDA_ABY, cpu.A, cpu.Y, true);
	}

	TEST_F(cpu6502_test, LDX_ABY)
	{
		load_AB(oc::LDX_ABY, cpu.X, cpu.Y);
	}

	TEST_F(cpu6502_test, LDX_ABY_C)
	{
		load_AB(oc::LDX_ABY, cpu.X, cpu.Y, true);
	}

	TEST_F(cpu6502_test, LDY_ABX)
	{
		load_AB(oc::LDY_ABX, cpu.Y, cpu.X);
	}

	TEST_F(cpu6502_test, LDY_ABX_C)
	{
		load_AB(oc::LDY_ABX, cpu.Y, cpu.X, true);
	}

	void cpu6502_test::load_IDX(oc opcode, uint8_t & reg)
	{
		addressing_IDX(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x0A);
		cpu.execute();

		EXPECT_EQ(reg, 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, LDA_IDX)
	{
		load_IDX(oc::LDA_IDX, cpu.A);
	}

	void cpu6502_test::load_IDY(oc opcode, uint8_t & reg, bool carry)
	{
		addressing_IDY(opcode, 0x80, 0x40, carry);
		bus.write(0x4080 + cpu.Y, 0x0B);
		cpu.execute();

		EXPECT_EQ(reg, 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
	}

	TEST_F(cpu6502_test, LDA_IDY)
	{
		load_IDY(oc::LDA_IDY, cpu.A);
	}

	TEST_F(cpu6502_test, LDA_IDY_C)
	{
		load_IDY(oc::LDA_IDY, cpu.A, true);
	}

	void cpu6502_test::store_ZPG(oc opcode, uint8_t & reg)
	{
		addressing_ZPG(opcode, 0x0A);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(0x000A), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_ZPG)
	{
		store_ZPG(oc::STA_ZPG, cpu.A);
	}

	TEST_F(cpu6502_test, STX_ZPG)
	{
		store_ZPG(oc::STX_ZPG, cpu.X);
	}

	TEST_F(cpu6502_test, STY_ZPG)
	{
		store_ZPG(oc::STY_ZPG, cpu.Y);
	}

	void cpu6502_test::store_ZP(oc opcode, uint8_t & reg, uint8_t & offset_reg)
	{
		addressing_ZP(opcode, offset_reg, 0x80);
		reg = 0x0C;
		cpu.execute();

		EXPECT_EQ(bus.read(0x0080 + offset_reg), 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_ZPX)
	{
		store_ZP(oc::STA_ZPX, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, STX_ZPY)
	{
		store_ZP(oc::STX_ZPY, cpu.X, cpu.Y);
	}

	TEST_F(cpu6502_test, STY_ZPX)
	{
		store_ZP(oc::STY_ZPX, cpu.Y, cpu.X);
	}

	void cpu6502_test::store_ABS(oc opcode, uint8_t & reg)
	{
		addressing_ABS(opcode, 0x80, 0x40);
		reg = 0x0D;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080), 0x0D);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_ABS)
	{
		store_ABS(oc::STA_ABS, cpu.A);
	}

	TEST_F(cpu6502_test, STX_ABS)
	{
		store_ABS(oc::STX_ABS, cpu.X);
	}

	TEST_F(cpu6502_test, STY_ABS)
	{
		store_ABS(oc::STY_ABS, cpu.Y);
	}

	void cpu6502_test::store_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg, bool carry)
	{
		addressing_AB(opcode, offset_reg, 0x80, 0x40, carry);
		reg = 0x0E;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080 + offset_reg), 0x0E);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_ABX)
	{
		store_AB(oc::STA_ABX, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, STA_ABX_C)
	{
		store_AB(oc::STA_ABX, cpu.A, cpu.X, true);
	}

	TEST_F(cpu6502_test, STA_ABY)
	{
		store_AB(oc::STA_ABY, cpu.A, cpu.Y);
	}

	TEST_F(cpu6502_test, STA_ABY_C)
	{
		store_AB(oc::STA_ABY, cpu.A, cpu.Y, true);
	}

	void cpu6502_test::store_IDX(oc opcode, uint8_t & reg)
	{
		addressing_IDX(opcode, 0x80, 0x40);
		reg = 0x0A;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080), 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_IDX)
	{
		store_IDX(oc::STA_IDX, cpu.A);
	}

	void cpu6502_test::store_IDY(oc opcode, uint8_t & reg, bool carry)
	{
		addressing_IDY(opcode, 0x80, 0x40, carry);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080 + cpu.Y), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_IDY)
	{
		store_IDY(oc::STA_IDY, cpu.A);
	}

	TEST_F(cpu6502_test, STA_IDY_C)
	{
		store_IDY(oc::STA_IDY, cpu.A, true);
	}

	void cpu6502_test::transfer(oc opcode, uint8_t & src, uint8_t & dst, bool check)
	{
		bus.write(address++, opcode);
		src = 0x0A;
		cpu.execute();

		EXPECT_EQ(src, dst);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

		if (check)
		{
			bus.write(address++, opcode);
			src = 0x00;
			cpu.execute();

			EXPECT_EQ(cpu.P.Z, 1);

			bus.write(address++, opcode);
			src = 0x80;
			cpu.execute();

			EXPECT_EQ(cpu.P.N, 1);
		}
	}

	TEST_F(cpu6502_test, TAX)
	{
		transfer(oc::TAX____, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, TAY)
	{
		transfer(oc::TAY____, cpu.A, cpu.Y);
	}

	TEST_F(cpu6502_test, TXA)
	{
		transfer(oc::TXA____, cpu.X, cpu.A);
	}

	TEST_F(cpu6502_test, TYA)
	{
		transfer(oc::TYA____, cpu.Y, cpu.A);
	}

	TEST_F(cpu6502_test, TSX)
	{
		transfer(oc::TSX____, cpu.S, cpu.X);
	}

	TEST_F(cpu6502_test, TXS)
	{
		transfer(oc::TXS____, cpu.X, cpu.S, false);
	}

	TEST_F(cpu6502_test, PHA)
	{
		auto opcode = oc::PHA____;

		bus.write(address++, opcode);
		cpu.A = 0x0B;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFE);
		EXPECT_EQ(bus.read(0x01FF), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, PHP)
	{
		auto opcode = oc::PHP____;

		bus.write(address++, opcode);
		cpu.P.N = 1;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFE);
		EXPECT_EQ(bus.read(0x01FF), 0x80);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, PLA)
	{
		auto opcode = oc::PLA____;

		bus.write(address++, opcode);
		bus.write(0x01FF, 0x0C);
		cpu.S = 0xFE;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFF);
		EXPECT_EQ(cpu.A, 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, PLP)
	{
		auto opcode = oc::PLP____;

		bus.write(address++, opcode);
		bus.write(0x01FF, 0x80);
		cpu.S = 0xFE;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFF);
		EXPECT_EQ(cpu.P.N, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	void cpu6502_test::logic(oc opcode, uint8_t a, uint8_t b)
	{
		cpu.A = a;
		cpu.execute();

		EXPECT_EQ(cpu.A, b);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, AND_IMM)
	{
		// 000000110 &
		// 000000011
		// ---------
		// 000000010

		auto opcode = oc::AND_IMM;

		addressing_IMM(opcode, 0x06);
		logic(opcode, 0x03, 0x02);

		addressing_IMM(opcode, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(opcode, 0x80);
		cpu.A = 0x80;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, AND_ZPG)
	{
		auto opcode = oc::AND_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ZPX)
	{
		auto opcode = oc::AND_ZPX;

		addressing_ZP(opcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABS)
	{
		auto opcode = oc::AND_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABX)
	{
		auto opcode = oc::AND_ABX;

		addressing_AB(opcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABY)
	{
		auto opcode = oc::AND_ABY;

		addressing_AB(opcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_IDX)
	{
		auto opcode = oc::AND_IDX;

		addressing_IDX(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_IDY)
	{
		auto opcode = oc::AND_IDY;

		addressing_IDY(opcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, EOR_IMM)
	{
		// 000000110 ^
		// 000000011
		// ---------
		// 000000101

		auto opcode = oc::EOR_IMM;

		addressing_IMM(opcode, 0x06);
		logic(opcode, 0x03, 0x05);

		addressing_IMM(opcode, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(opcode, 0x80);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, EOR_ZPG)
	{
		auto opcode = oc::EOR_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ZPX)
	{
		auto opcode = oc::EOR_ZPX;

		addressing_ZP(opcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABS)
	{
		auto opcode = oc::EOR_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABX)
	{
		auto opcode = oc::EOR_ABX;

		addressing_AB(opcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABY)
	{
		auto opcode = oc::EOR_ABY;

		addressing_AB(opcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_IDX)
	{
		auto opcode = oc::EOR_IDX;

		addressing_IDX(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_IDY)
	{
		auto opcode = oc::EOR_IDY;

		addressing_IDY(opcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, ORA_IMM)
	{
		// 000000110 |
		// 000000011
		// ---------
		// 000000111

		auto opcode = oc::ORA_IMM;

		addressing_IMM(opcode, 0x06);
		logic(opcode, 0x03, 0x07);

		addressing_IMM(opcode, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(opcode, 0x80);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, ORA_ZPG)
	{
		auto opcode = oc::ORA_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ZPX)
	{
		auto opcode = oc::ORA_ZPX;

		addressing_ZP(opcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABS)
	{
		auto opcode = oc::ORA_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABX)
	{
		auto opcode = oc::ORA_ABX;

		addressing_AB(opcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABY)
	{
		auto opcode = oc::ORA_ABY;

		addressing_AB(opcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_IDX)
	{
		auto opcode = oc::ORA_IDX;

		addressing_IDX(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_IDY)
	{
		auto opcode = oc::ORA_IDY;

		addressing_IDY(opcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, BIT_ZPG)
	{
		auto opcode = oc::BIT_ZPG;

		addressing_ZPG(opcode, 0x0A);
		bus.write(0x000A, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

		addressing_ZPG(opcode, 0x0A);
		bus.write(0x000A, 0xFE);
		cpu.A = 0xFF;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.V, 1);
		EXPECT_EQ(cpu.P.N, 1);
		EXPECT_EQ(cpu.A, 0xFF);
	}

	TEST_F(cpu6502_test, BIT_ABS)
	{
		auto opcode = oc::BIT_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0xFE);
		cpu.A = 0xFF;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.V, 1);
		EXPECT_EQ(cpu.P.N, 1);
		EXPECT_EQ(cpu.A, 0xFF);
	}

	TEST_F(cpu6502_test, ADC_IMM_flag_C_Z)
	{
		auto opcode = oc::ADC_IMM;

		addressing_IMM(opcode, 0xFF);
		cpu.A = 0x01;
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 1);
	}

	TEST_F(cpu6502_test, ADC_IMM_flag_N)
	{
		auto opcode = oc::ADC_IMM;

		addressing_IMM(opcode, 0x7F);
		cpu.A = 0x01;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, ADC_IMM_flag_V_both_positive)
	{
		auto opcode = oc::ADC_IMM;

		addressing_IMM(opcode, 0x40);
		cpu.A = 0x40;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 1);
	}

	TEST_F(cpu6502_test, ADC_IMM_flag_V_both_negative)
	{
		auto opcode = oc::ADC_IMM;

		addressing_IMM(opcode, 0x80);
		cpu.A = 0x80;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 1);
	}

	TEST_F(cpu6502_test, ADC_IMM_flag_V_positive_negative)
	{
		auto opcode = oc::ADC_IMM;

		addressing_IMM(opcode, 0x80);
		cpu.A = 0x40;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 0);
	}

	TEST_F(cpu6502_test, ADC_IMM_flag_V_negative_positive)
	{
		auto opcode = oc::ADC_IMM;

		addressing_IMM(opcode, 0x40);
		cpu.A = 0x80;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 0);
	}

	void cpu6502_test::add(oc opcode)
	{
		cpu.A = 0x02;
		cpu.P.C = 1;
		cpu.execute();

		EXPECT_EQ(cpu.A, 0x04);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, ADC_IMM)
	{
		auto opcode = oc::ADC_IMM;

		addressing_IMM(opcode, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ZPG)
	{
		auto opcode = oc::ADC_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ZPX)
	{
		auto opcode = oc::ADC_ZPX;

		addressing_ZP(opcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABS)
	{
		auto opcode = oc::ADC_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABX)
	{
		auto opcode = oc::ADC_ABX;

		addressing_AB(opcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABY)
	{
		auto opcode = oc::ADC_ABY;

		addressing_AB(opcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_IDX)
	{
		auto opcode = oc::ADC_IDX;

		addressing_IDX(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_IDY)
	{
		auto opcode = oc::ADC_IDY;

		addressing_IDY(opcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, SBC_IMM_flag_C_Z)
	{
		auto opcode = oc::SBC_IMM;

		addressing_IMM(opcode, 0x04);
		cpu.A = 0x05;
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 1);
	}

	TEST_F(cpu6502_test, SBC_IMM_flag_N)
	{
		auto opcode = oc::SBC_IMM;

		addressing_IMM(opcode, 0x7E);
		cpu.A = 0x01;
		cpu.P.C = 1;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, SBC_IMM_flag_V_both_positive)
	{
		auto opcode = oc::SBC_IMM;

		addressing_IMM(opcode, 0x40);
		cpu.A = 0x40;
		cpu.P.C = 1;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 0);
	}

	TEST_F(cpu6502_test, SBC_IMM_flag_V_both_negative)
	{
		auto opcode = oc::SBC_IMM;

		addressing_IMM(opcode, 0x80);
		cpu.A = 0x80;
		cpu.P.C = 1;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 0);
	}

	TEST_F(cpu6502_test, SBC_IMM_flag_V_positive_negative)
	{
		auto opcode = oc::SBC_IMM;
		addressing_IMM(opcode, 0x70);
		cpu.A = 0xD0;
		cpu.P.C = 1;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 1);
	}

	TEST_F(cpu6502_test, SBC_IMM_flag_V_negative_positive)
	{
		auto opcode = oc::SBC_IMM;

		addressing_IMM(opcode, 0x90);
		cpu.A = 0x10;
		cpu.P.C = 1;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 1);
	}

	void cpu6502_test::substract(oc opcode)
	{
		cpu.A = 0x05;
		cpu.P.C = 0;
		cpu.execute();

		EXPECT_EQ(cpu.A, 0x02);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, SBC_IMM)
	{
		auto opcode = oc::SBC_IMM;

		addressing_IMM(opcode, 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ZPG)
	{
		auto opcode = oc::SBC_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ZPX)
	{
		auto opcode = oc::SBC_ZPX;

		addressing_ZP(opcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABS)
	{
		auto opcode = oc::SBC_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABX)
	{
		auto opcode = oc::SBC_ABX;

		addressing_AB(opcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABY)
	{
		auto opcode = oc::SBC_ABY;

		addressing_AB(opcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_IDX)
	{
		auto opcode = oc::SBC_IDX;

		addressing_IDX(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_IDY)
	{
		auto opcode = oc::SBC_IDY;

		addressing_IDY(opcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x02);
		substract(opcode);
	}

	void cpu6502_test::compare(oc opcode, uint8_t & reg)
	{
		reg = 0x05;
		cpu.P.C = 0;
		cpu.execute();

		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, CMP_IMM)
	{
		auto opcode = oc::CMP_IMM;

		addressing_IMM(opcode, 0x02);
		compare(opcode, cpu.A);

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.N, 0);
		
		addressing_IMM(opcode, 0x05);
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(cpu.P.N, 0);

		addressing_IMM(opcode, 0x07);
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 0);
		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, CMP_ZPG)
	{
		auto opcode = oc::CMP_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ZPX)
	{
		auto opcode = oc::CMP_ZPX;

		addressing_ZP(opcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABS)
	{
		auto opcode = oc::CMP_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABX)
	{
		auto opcode = oc::CMP_ABX;

		addressing_AB(opcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABY)
	{
		auto opcode = oc::CMP_ABY;

		addressing_AB(opcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_IDX)
	{
		auto opcode = oc::CMP_IDX;

		addressing_IDX(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_IDY)
	{
		auto opcode = oc::CMP_IDY;

		addressing_IDY(opcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CPX_IMM)
	{
		auto opcode = oc::CPX_IMM;

		addressing_IMM(opcode, 0x02);
		compare(opcode, cpu.X);

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.N, 0);

		addressing_IMM(opcode, 0x05);
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(cpu.P.N, 0);

		addressing_IMM(opcode, 0x07);
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 0);
		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, CPX_ZPG)
	{
		auto opcode = oc::CPX_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x02);
		compare(opcode, cpu.X);
	}

	TEST_F(cpu6502_test, CPX_ABS)
	{
		auto opcode = oc::CPX_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x02);
		compare(opcode, cpu.X);
	}

	TEST_F(cpu6502_test, CPY_IMM)
	{
		auto opcode = oc::CPY_IMM;

		addressing_IMM(opcode, 0x02);
		compare(opcode, cpu.Y);

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.N, 0);

		addressing_IMM(opcode, 0x05);
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(cpu.P.N, 0);

		addressing_IMM(opcode, 0x07);
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 0);
		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, CPY_ZPG)
	{
		auto opcode = oc::CPY_ZPG;

		addressing_ZPG(opcode, 0x80);
		bus.write(0x0080, 0x02);
		compare(opcode, cpu.Y);
	}

	TEST_F(cpu6502_test, CPY_ABS)
	{
		auto opcode = oc::CPY_ABS;

		addressing_ABS(opcode, 0x80, 0x40);
		bus.write(0x4080, 0x02);
		compare(opcode, cpu.Y);
	}
}