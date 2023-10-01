#include "gtest/gtest.h"
#include "../cpu6502/cpu6502.h"

namespace emulator
{
	class cpu6502_test : public testing::Test
	{
	public:
		using oc = emulator::cpu6502::oppcode;

		emulator::clock clock;
		emulator::bus bus;
		emulator::cpu6502 cpu { clock, bus };
		uint16_t address = {};

		void SetUp() override
		{
			clock.reset();
			bus.reset();
			cpu.reset();
			address = 0x0200;
		}

		void addressing_IMM(oc oppcode, uint8_t data)
		{
			bus.write(address++, oppcode);
			bus.write(address++, data);
		}

		void addressing_ZPG(oc oppcode, uint8_t lo)
		{
			bus.write(address++, oppcode);
			bus.write(address++, lo);
		}

		void addressing_ZP(oc oppcode, uint8_t & offset_reg, uint8_t lo)
		{
			offset_reg = 0x0F;
			bus.write(address++, oppcode);
			bus.write(address++, lo);
		}

		void addressing_ABS(oc oppcode, uint8_t lo, uint8_t hi)
		{
			bus.write(address++, oppcode);
			bus.write(address++, lo);
			bus.write(address++, hi);
		}

		void addressing_AB(oc oppcode, uint8_t & offset_reg, uint8_t lo, uint8_t hi, bool carry = false)
		{
			if (carry)
				offset_reg = 0xFF;
			else
				offset_reg = 0x0F;

			bus.write(address++, oppcode);
			bus.write(address++, lo);
			bus.write(address++, hi);
		}

		void addressing_IDX(oc oppcode, uint8_t lo, uint8_t hi)
		{
			cpu.X = 0x0F;
			bus.write(address++, oppcode);
			bus.write(address++, 0x20);
			bus.write(0x002F, lo);
			bus.write(0x0030, hi);
		}

		void addressing_IDY(oc oppcode, uint8_t lo, uint8_t hi, bool carry = false)
		{
			if (carry)
				cpu.Y = 0xFF;
			else
				cpu.Y = 0x0F;

			bus.write(address++, oppcode);
			bus.write(address++, 0x20);
			bus.write(0x0020, lo);
			bus.write(0x0021, hi);
		}

		void load_IMM(oc oppcode, uint8_t & reg);
		void load_ZPG(oc oppcode, uint8_t & reg);
		void load_ZP(oc oppcode, uint8_t & reg, uint8_t & offset_reg);
		void load_ABS(oc oppcode, uint8_t & reg);
		void load_AB(oc oppcode, uint8_t & reg, uint8_t & offset_reg, bool carry = false);
		void load_IDX(oc oppcode, uint8_t & reg);
		void load_IDY(oc oppcode, uint8_t & reg, bool carry = false);

		void store_ZPG(oc oppcode, uint8_t & reg);
		void store_ZP(oc oppcode, uint8_t & reg, uint8_t & offset_reg);
		void store_ABS(oc oppcode, uint8_t & reg);
		void store_AB(oc oppcode, uint8_t & reg, uint8_t & offset_reg, bool carry = false);
		void store_IDX(oc oppcode, uint8_t & reg);
		void store_IDY(oc oppcode, uint8_t & reg, bool carry = false);

		void transfer(oc oppcode, uint8_t & src, uint8_t & dst, bool check = true);
		void logic(oc oppcode, uint8_t a, uint8_t b);
	};

	void cpu6502_test::load_IMM(oc oppcode, uint8_t & reg)
	{
		addressing_IMM(oppcode, 0x0A);
		cpu.execute();

		EXPECT_EQ(reg, 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		addressing_IMM(oppcode, 0x00);
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(oppcode, 0x80);
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, LDA_IMM_ZN)
	{
		load_IMM(oc::LDA_IMM, cpu.A);
	}

	TEST_F(cpu6502_test, LDX_IMM_ZN)
	{
		load_IMM(oc::LDX_IMM, cpu.X);
	}

	TEST_F(cpu6502_test, LDY_IMM_ZN)
	{
		load_IMM(oc::LDY_IMM, cpu.Y);
	}

	void cpu6502_test::load_ZPG(oc oppcode, uint8_t & reg)
	{
		addressing_ZPG(oppcode, 0x0A);
		bus.write(0x000A, 0x0B);
		cpu.execute();

		EXPECT_EQ(reg, 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
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

	void cpu6502_test::load_ZP(oc oppcode, uint8_t & reg, uint8_t & offset_reg)
	{
		addressing_ZP(oppcode, offset_reg, 0x80);
		bus.write(0x0080 + offset_reg, 0x0C);
		cpu.execute();

		EXPECT_EQ(reg, 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
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

	void cpu6502_test::load_ABS(oc oppcode, uint8_t & reg)
	{
		addressing_ABS(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x0D);
		cpu.execute();

		EXPECT_EQ(reg, 0x0D);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
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

	void cpu6502_test::load_AB(oc oppcode, uint8_t & reg, uint8_t & offset_reg, bool carry)
	{
		addressing_AB(oppcode, offset_reg, 0x80, 0x40, carry);
		bus.write(0x4080 + offset_reg, 0x0E);
		cpu.execute();

		EXPECT_EQ(reg, 0x0E);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles + carry);
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

	void cpu6502_test::load_IDX(oc oppcode, uint8_t & reg)
	{
		addressing_IDX(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x0A);
		cpu.execute();

		EXPECT_EQ(reg, 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	TEST_F(cpu6502_test, LDA_IDX)
	{
		load_IDX(oc::LDA_IDX, cpu.A);
	}

	void cpu6502_test::load_IDY(oc oppcode, uint8_t & reg, bool carry)
	{
		addressing_IDY(oppcode, 0x80, 0x40, carry);
		bus.write(0x4080 + cpu.Y, 0x0B);
		cpu.execute();

		EXPECT_EQ(reg, 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles + carry);
	}

	TEST_F(cpu6502_test, LDA_IDY)
	{
		load_IDY(oc::LDA_IDY, cpu.A);
	}

	TEST_F(cpu6502_test, LDA_IDY_C)
	{
		load_IDY(oc::LDA_IDY, cpu.A, true);
	}

	void cpu6502_test::store_ZPG(oc oppcode, uint8_t & reg)
	{
		addressing_ZPG(oppcode, 0x0A);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(0x000A), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
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

	void cpu6502_test::store_ZP(oc oppcode, uint8_t & reg, uint8_t & offset_reg)
	{
		addressing_ZP(oppcode, offset_reg, 0x80);
		reg = 0x0C;
		cpu.execute();

		EXPECT_EQ(bus.read(0x0080 + offset_reg), 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
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

	void cpu6502_test::store_ABS(oc oppcode, uint8_t & reg)
	{
		addressing_ABS(oppcode, 0x80, 0x40);
		reg = 0x0D;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080), 0x0D);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
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

	void cpu6502_test::store_AB(oc oppcode, uint8_t & reg, uint8_t & offset_reg, bool carry)
	{
		addressing_AB(oppcode, offset_reg, 0x80, 0x40, carry);
		reg = 0x0E;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080 + offset_reg), 0x0E);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
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

	void cpu6502_test::store_IDX(oc oppcode, uint8_t & reg)
	{
		addressing_IDX(oppcode, 0x80, 0x40);
		reg = 0x0A;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080), 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	TEST_F(cpu6502_test, STA_IDX)
	{
		store_IDX(oc::STA_IDX, cpu.A);
	}

	void cpu6502_test::store_IDY(oc oppcode, uint8_t & reg, bool carry)
	{
		addressing_IDY(oppcode, 0x80, 0x40, carry);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(0x4080 + cpu.Y), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	TEST_F(cpu6502_test, STA_IDY)
	{
		store_IDY(oc::STA_IDY, cpu.A);
	}

	TEST_F(cpu6502_test, STA_IDY_C)
	{
		store_IDY(oc::STA_IDY, cpu.A, true);
	}

	void cpu6502_test::transfer(oc oppcode, uint8_t & src, uint8_t & dst, bool check)
	{
		bus.write(address++, oppcode);
		src = 0x0A;
		cpu.execute();

		EXPECT_EQ(src, dst);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		if (check)
		{
			bus.write(address++, oppcode);
			src = 0x00;
			cpu.execute();

			EXPECT_EQ(cpu.P.Z, 1);

			bus.write(address++, oppcode);
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
		auto oppcode = oc::PHA____;

		bus.write(address++, oppcode);
		cpu.A = 0x0B;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFE);
		EXPECT_EQ(bus.read(0x00FF), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	TEST_F(cpu6502_test, PHP)
	{
		auto oppcode = oc::PHP____;

		bus.write(address++, oppcode);
		cpu.P.N = 1;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFE);
		EXPECT_EQ(bus.read(0x00FF), 0x80);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	TEST_F(cpu6502_test, PLA)
	{
		auto oppcode = oc::PLA____;

		bus.write(address++, oppcode);
		bus.write(0x00FF, 0x0C);
		cpu.S = 0xFE;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFF);
		EXPECT_EQ(cpu.A, 0x0C);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	TEST_F(cpu6502_test, PLP)
	{
		auto oppcode = oc::PLP____;

		bus.write(address++, oppcode);
		bus.write(0x00FF, 0x80);
		cpu.S = 0xFE;
		cpu.execute();

		EXPECT_EQ(cpu.S, 0xFF);
		EXPECT_EQ(cpu.P.N, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	void cpu6502_test::logic(oc oppcode, uint8_t a, uint8_t b)
	{
		cpu.A = a;
		cpu.execute();

		EXPECT_EQ(cpu.A, b);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);
	}

	TEST_F(cpu6502_test, AND_IMM_ZN)
	{
		// 000000110 &
		// 000000011
		// ---------
		// 000000010

		auto oppcode = oc::AND_IMM;

		addressing_IMM(oppcode, 0x06);
		logic(oppcode, 0x03, 0x02);

		addressing_IMM(oppcode, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(oppcode, 0x80);
		cpu.A = 0x80;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, AND_ZPG)
	{
		auto oppcode = oc::AND_ZPG;

		addressing_ZPG(oppcode, 0x80);
		bus.write(0x0080, 0x06);
		logic(oppcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ZPX)
	{
		auto oppcode = oc::AND_ZPX;

		addressing_ZP(oppcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x06);
		logic(oppcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABS)
	{
		auto oppcode = oc::AND_ABS;

		addressing_ABS(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(oppcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABX)
	{
		auto oppcode = oc::AND_ABX;

		addressing_AB(oppcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x06);
		logic(oppcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABY)
	{
		auto oppcode = oc::AND_ABY;

		addressing_AB(oppcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(oppcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_IDX)
	{
		auto oppcode = oc::AND_IDX;

		addressing_IDX(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(oppcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_IDY)
	{
		auto oppcode = oc::AND_IDY;

		addressing_IDY(oppcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(oppcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, EOR_IMM_ZN)
	{
		// 000000110 ^
		// 000000011
		// ---------
		// 000000101

		auto oppcode = oc::EOR_IMM;

		addressing_IMM(oppcode, 0x06);
		logic(oppcode, 0x03, 0x05);

		addressing_IMM(oppcode, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(oppcode, 0x80);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, EOR_ZPG)
	{
		auto oppcode = oc::EOR_ZPG;

		addressing_ZPG(oppcode, 0x80);
		bus.write(0x0080, 0x06);
		logic(oppcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ZPX)
	{
		auto oppcode = oc::EOR_ZPX;

		addressing_ZP(oppcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x06);
		logic(oppcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABS)
	{
		auto oppcode = oc::EOR_ABS;

		addressing_ABS(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(oppcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABX)
	{
		auto oppcode = oc::EOR_ABX;

		addressing_AB(oppcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x06);
		logic(oppcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABY)
	{
		auto oppcode = oc::EOR_ABY;

		addressing_AB(oppcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(oppcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_IDX)
	{
		auto oppcode = oc::EOR_IDX;

		addressing_IDX(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(oppcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_IDY)
	{
		auto oppcode = oc::EOR_IDY;

		addressing_IDY(oppcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(oppcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, ORA_IMM_ZN)
	{
		// 000000110 |
		// 000000011
		// ---------
		// 000000111

		auto oppcode = oc::ORA_IMM;

		addressing_IMM(oppcode, 0x06);
		logic(oppcode, 0x03, 0x07);

		addressing_IMM(oppcode, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(oppcode, 0x80);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);
	}

	TEST_F(cpu6502_test, ORA_ZPG)
	{
		auto oppcode = oc::ORA_ZPG;

		addressing_ZPG(oppcode, 0x80);
		bus.write(0x0080, 0x06);
		logic(oppcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ZPX)
	{
		auto oppcode = oc::ORA_ZPX;

		addressing_ZP(oppcode, cpu.X, 0x80);
		bus.write(0x0080 + cpu.X, 0x06);
		logic(oppcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABS)
	{
		auto oppcode = oc::ORA_ABS;

		addressing_ABS(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(oppcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABX)
	{
		auto oppcode = oc::ORA_ABX;

		addressing_AB(oppcode, cpu.X, 0x80, 0x40);
		bus.write(0x4080 + cpu.X, 0x06);
		logic(oppcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABY)
	{
		auto oppcode = oc::ORA_ABY;

		addressing_AB(oppcode, cpu.Y, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(oppcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_IDX)
	{
		auto oppcode = oc::ORA_IDX;

		addressing_IDX(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x06);
		logic(oppcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_IDY)
	{
		auto oppcode = oc::ORA_IDY;

		addressing_IDY(oppcode, 0x80, 0x40);
		bus.write(0x4080 + cpu.Y, 0x06);
		logic(oppcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, BIT_ZPG)
	{
		auto oppcode = oc::BIT_ZPG;

		addressing_ZPG(oppcode, 0x0A);
		bus.write(0x000A, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		addressing_ZPG(oppcode, 0x0A);
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
		auto oppcode = oc::BIT_ABS;

		addressing_ABS(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		addressing_ABS(oppcode, 0x80, 0x40);
		bus.write(0x4080, 0xFE);
		cpu.A = 0xFF;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 0);
		EXPECT_EQ(cpu.P.V, 1);
		EXPECT_EQ(cpu.P.N, 1);
		EXPECT_EQ(cpu.A, 0xFF);
	}

	TEST_F(cpu6502_test, ADC_IMM)
	{
		auto oppcode = oc::ADC_IMM;

		addressing_IMM(oppcode, 0x01);
		cpu.A = 0x02;
		cpu.P.C = 1;
		cpu.execute();

		EXPECT_EQ(cpu.A, 0x04);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(oppcode).cycles);

		addressing_IMM(oppcode, 0xFF);
		cpu.A = 0x01;
		cpu.P.C = 0;
		cpu.execute();

		EXPECT_EQ(cpu.P.C, 1);
		EXPECT_EQ(cpu.P.Z, 1);

		addressing_IMM(oppcode, 0x7F);
		cpu.A = 0x01;
		cpu.P.C = 0;
		cpu.execute();

		EXPECT_EQ(cpu.P.N, 1);

		addressing_IMM(oppcode, 0x40);
		cpu.A = 0x40;
		cpu.P.C = 0;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 1);

		addressing_IMM(oppcode, 0x80);
		cpu.A = 0x80;
		cpu.P.C = 0;
		cpu.P.V = 0;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 1);

		addressing_IMM(oppcode, 0x80);
		cpu.A = 0x40;
		cpu.P.C = 0;
		cpu.P.V = 0;
		cpu.execute();

		EXPECT_EQ(cpu.P.V, 0);
	}
}