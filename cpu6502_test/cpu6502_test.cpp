#include "cpu6502_test.h"

namespace emulator
{
	void cpu6502_test::SetUp()
	{
		clock.reset();
		bus.reset();
		cpu.reset();
		address = 0x0200;
		carry = false;
	}

	void cpu6502_test::addressing_IMM(oc opcode, uint8_t data)
	{
		bus.write(address++, opcode);
		bus.write(address++, data);
	}

	uint16_t cpu6502_test::addressing_ZPG(oc opcode)
	{
		bus.write(address++, opcode);
		bus.write(address++, 0x80);

		return 0x80;
	}

	uint16_t cpu6502_test::addressing_ZP(oc opcode, uint8_t & offset_reg)
	{
		offset_reg = 0x0F;
		bus.write(address++, opcode);
		bus.write(address++, 0x80);

		return 0x80 + offset_reg;
	}

	uint16_t cpu6502_test::addressing_ABS(oc opcode)
	{
		bus.write(address++, opcode);
		bus.write(address++, 0x80);
		bus.write(address++, 0x40);

		return 0x4080;
	}

	uint16_t cpu6502_test::addressing_AB(oc opcode, uint8_t & offset_reg)
	{
		if (carry)
			offset_reg = 0xFF;
		else
			offset_reg = 0x0F;

		bus.write(address++, opcode);
		bus.write(address++, 0x80);
		bus.write(address++, 0x40);

		return 0x4080 + offset_reg;
	}

	uint16_t cpu6502_test::addressing_IDX(oc opcode)
	{
		cpu.X = 0x0F;
		bus.write(address++, opcode);
		bus.write(address++, 0x20);
		bus.write(0x002F, 0x80);
		bus.write(0x0030, 0x40);

		return 0x4080;
	}

	uint16_t cpu6502_test::addressing_IDY(oc opcode)
	{
		if (carry)
			cpu.Y = 0xFF;
		else
			cpu.Y = 0x0F;

		bus.write(address++, opcode);
		bus.write(address++, 0x20);
		bus.write(0x0020, 0x80);
		bus.write(0x0021, 0x40);

		return 0x4080 + cpu.Y;
	}

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
		bus.write(addressing_ZPG(opcode), 0x0B);
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
		bus.write(addressing_ZP(opcode, offset_reg), 0x0C);
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
		bus.write(addressing_ABS(opcode), 0x0D);
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

	void cpu6502_test::load_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg)
	{
		bus.write(addressing_AB(opcode, offset_reg), 0x0E);
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
		carry = true;
		load_AB(oc::LDA_ABX, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, LDA_ABY)
	{
		load_AB(oc::LDA_ABY, cpu.A, cpu.Y);
	}

	TEST_F(cpu6502_test, LDA_ABY_C)
	{
		carry = true;
		load_AB(oc::LDA_ABY, cpu.A, cpu.Y);
	}

	TEST_F(cpu6502_test, LDX_ABY)
	{
		load_AB(oc::LDX_ABY, cpu.X, cpu.Y);
	}

	TEST_F(cpu6502_test, LDX_ABY_C)
	{
		carry = true;
		load_AB(oc::LDX_ABY, cpu.X, cpu.Y);
	}

	TEST_F(cpu6502_test, LDY_ABX)
	{
		load_AB(oc::LDY_ABX, cpu.Y, cpu.X);
	}

	TEST_F(cpu6502_test, LDY_ABX_C)
	{
		carry = true;
		load_AB(oc::LDY_ABX, cpu.Y, cpu.X);
	}

	void cpu6502_test::load_IDX(oc opcode, uint8_t & reg)
	{
		bus.write(addressing_IDX(opcode), 0x0A);
		cpu.execute();

		EXPECT_EQ(reg, 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, LDA_IDX)
	{
		load_IDX(oc::LDA_IDX, cpu.A);
	}

	void cpu6502_test::load_IDY(oc opcode, uint8_t & reg)
	{
		bus.write(addressing_IDY(opcode), 0x0B);
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
		carry = true;
		load_IDY(oc::LDA_IDY, cpu.A);
	}

	void cpu6502_test::store_ZPG(oc opcode, uint8_t & reg)
	{
		auto addr = addressing_ZPG(opcode);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x0B);
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
		auto addr = addressing_ZP(opcode, offset_reg);
		reg = 0x0C;
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x0C);
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
		auto addr = addressing_ABS(opcode);
		reg = 0x0D;
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x0D);
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

	void cpu6502_test::store_AB(oc opcode, uint8_t & reg, uint8_t & offset_reg)
	{
		auto addr = addressing_AB(opcode, offset_reg);
		reg = 0x0E;
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x0E);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_ABX)
	{
		store_AB(oc::STA_ABX, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, STA_ABX_C)
	{
		carry = true;
		store_AB(oc::STA_ABX, cpu.A, cpu.X);
	}

	TEST_F(cpu6502_test, STA_ABY)
	{
		store_AB(oc::STA_ABY, cpu.A, cpu.Y);
	}

	TEST_F(cpu6502_test, STA_ABY_C)
	{
		carry = true;
		store_AB(oc::STA_ABY, cpu.A, cpu.Y);
	}

	void cpu6502_test::store_IDX(oc opcode, uint8_t & reg)
	{
		auto addr = addressing_IDX(opcode);
		reg = 0x0A;
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x0A);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_IDX)
	{
		store_IDX(oc::STA_IDX, cpu.A);
	}

	void cpu6502_test::store_IDY(oc opcode, uint8_t & reg)
	{
		auto addr = addressing_IDY(opcode);
		reg = 0x0B;
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x0B);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, STA_IDY)
	{
		store_IDY(oc::STA_IDY, cpu.A);
	}

	TEST_F(cpu6502_test, STA_IDY_C)
	{
		carry = true;
		store_IDY(oc::STA_IDY, cpu.A);
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
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
	}

	TEST_F(cpu6502_test, AND_IMM)
	{
		// 000000110 & 6
		// 000000011   3
		// ---------
		// 000000010   2

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

		bus.write(addressing_ZPG(opcode), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ZPX)
	{
		auto opcode = oc::AND_ZPX;

		bus.write(addressing_ZP(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABS)
	{
		auto opcode = oc::AND_ABS;

		bus.write(addressing_ABS(opcode), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABX)
	{
		auto opcode = oc::AND_ABX;

		bus.write(addressing_AB(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABX_C)
	{
		auto opcode = oc::AND_ABX;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABY)
	{
		auto opcode = oc::AND_ABY;

		bus.write(addressing_AB(opcode, cpu.Y), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_ABY_C)
	{
		auto opcode = oc::AND_ABY;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.Y), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_IDX)
	{
		auto opcode = oc::AND_IDX;

		bus.write(addressing_IDX(opcode), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_IDY)
	{
		auto opcode = oc::AND_IDY;

		bus.write(addressing_IDY(opcode), 0x06);
		logic(opcode, 0x03, 0x02);
	}

	TEST_F(cpu6502_test, AND_IDY_C)
	{
		auto opcode = oc::AND_IDY;

		carry = true;
		bus.write(addressing_IDY(opcode), 0x06);
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

		bus.write(addressing_ZPG(opcode), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ZPX)
	{
		auto opcode = oc::EOR_ZPX;

		bus.write(addressing_ZP(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABS)
	{
		auto opcode = oc::EOR_ABS;

		bus.write(addressing_ABS(opcode), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABX)
	{
		auto opcode = oc::EOR_ABX;

		bus.write(addressing_AB(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABX_C)
	{
		auto opcode = oc::EOR_ABX;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABY)
	{
		auto opcode = oc::EOR_ABY;

		bus.write(addressing_AB(opcode, cpu.Y), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_ABY_C)
	{
		auto opcode = oc::EOR_ABY;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.Y), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_IDX)
	{
		auto opcode = oc::EOR_IDX;

		bus.write(addressing_IDX(opcode), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_IDY)
	{
		auto opcode = oc::EOR_IDY;

		bus.write(addressing_IDY(opcode), 0x06);
		logic(opcode, 0x03, 0x05);
	}

	TEST_F(cpu6502_test, EOR_IDY_C)
	{
		auto opcode = oc::EOR_IDY;

		carry = true;
		bus.write(addressing_IDY(opcode), 0x06);
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

		bus.write(addressing_ZPG(opcode), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ZPX)
	{
		auto opcode = oc::ORA_ZPX;

		bus.write(addressing_ZP(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABS)
	{
		auto opcode = oc::ORA_ABS;

		bus.write(addressing_ABS(opcode), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABX)
	{
		auto opcode = oc::ORA_ABX;

		bus.write(addressing_AB(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABX_C)
	{
		auto opcode = oc::ORA_ABX;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.X), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABY)
	{
		auto opcode = oc::ORA_ABY;

		bus.write(addressing_AB(opcode, cpu.Y), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_ABY_C)
	{
		auto opcode = oc::ORA_ABY;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.Y), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_IDX)
	{
		auto opcode = oc::ORA_IDX;

		bus.write(addressing_IDX(opcode), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, ORA_IDY)
	{
		auto opcode = oc::ORA_IDY;

		bus.write(addressing_IDY(opcode), 0x06);
		logic(opcode, 0x03, 0x07);
	}

	TEST_F(cpu6502_test, BIT_ZPG)
	{
		auto opcode = oc::BIT_ZPG;

		bus.write(addressing_ZPG(opcode), 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

		bus.write(addressing_ZPG(opcode), 0xFE);
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

		bus.write(addressing_ABS(opcode), 0x00);
		cpu.A = 0x00;
		cpu.execute();

		EXPECT_EQ(cpu.P.Z, 1);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);

		bus.write(addressing_ABS(opcode), 0xFE);
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
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
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

		bus.write(addressing_ZPG(opcode), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ZPX)
	{
		auto opcode = oc::ADC_ZPX;

		bus.write(addressing_ZP(opcode, cpu.X), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABS)
	{
		auto opcode = oc::ADC_ABS;

		bus.write(addressing_ABS(opcode), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABX)
	{
		auto opcode = oc::ADC_ABX;

		bus.write(addressing_AB(opcode, cpu.X), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABX_C)
	{
		auto opcode = oc::ADC_ABX;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.X), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABY)
	{
		auto opcode = oc::ADC_ABY;

		bus.write(addressing_AB(opcode, cpu.Y), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_ABY_C)
	{
		auto opcode = oc::ADC_ABY;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.Y), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_IDX)
	{
		auto opcode = oc::ADC_IDX;

		bus.write(addressing_IDX(opcode), 0x01);
		add(opcode);
	}

	TEST_F(cpu6502_test, ADC_IDY)
	{
		auto opcode = oc::ADC_IDY;

		bus.write(addressing_IDY(opcode), 0x01);
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
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
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

		bus.write(addressing_ZPG(opcode), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ZPX)
	{
		auto opcode = oc::SBC_ZPX;

		bus.write(addressing_ZP(opcode, cpu.X), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABS)
	{
		auto opcode = oc::SBC_ABS;

		bus.write(addressing_ABS(opcode), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABX)
	{
		auto opcode = oc::SBC_ABX;

		bus.write(addressing_AB(opcode, cpu.X), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABX_C)
	{
		auto opcode = oc::SBC_ABX;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.X), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABY)
	{
		auto opcode = oc::SBC_ABY;

		bus.write(addressing_AB(opcode, cpu.Y), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_ABY_C)
	{
		auto opcode = oc::SBC_ABY;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.Y), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_IDX)
	{
		auto opcode = oc::SBC_IDX;

		bus.write(addressing_IDX(opcode), 0x02);
		substract(opcode);
	}

	TEST_F(cpu6502_test, SBC_IDY)
	{
		auto opcode = oc::SBC_IDY;

		bus.write(addressing_IDY(opcode), 0x02);
		substract(opcode);
	}

	void cpu6502_test::compare(oc opcode, uint8_t & reg)
	{
		reg = 0x05;
		cpu.P.C = 0;
		cpu.execute();

		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles + carry);
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

		bus.write(addressing_ZPG(opcode), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ZPX)
	{
		auto opcode = oc::CMP_ZPX;

		bus.write(addressing_ZP(opcode, cpu.X), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABS)
	{
		auto opcode = oc::CMP_ABS;

		bus.write(addressing_ABS(opcode), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABX)
	{
		auto opcode = oc::CMP_ABX;

		bus.write(addressing_AB(opcode, cpu.X), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABX_C)
	{
		auto opcode = oc::CMP_ABX;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.X), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABY)
	{
		auto opcode = oc::CMP_ABY;

		bus.write(addressing_AB(opcode, cpu.Y), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_ABY_C)
	{
		auto opcode = oc::CMP_ABY;

		carry = true;
		bus.write(addressing_AB(opcode, cpu.Y), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_IDX)
	{
		auto opcode = oc::CMP_IDX;

		bus.write(addressing_IDX(opcode), 0x02);
		compare(opcode, cpu.A);
	}

	TEST_F(cpu6502_test, CMP_IDY)
	{
		auto opcode = oc::CMP_IDY;

		bus.write(addressing_IDY(opcode), 0x02);
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

		bus.write(addressing_ZPG(opcode), 0x02);
		compare(opcode, cpu.X);
	}

	TEST_F(cpu6502_test, CPX_ABS)
	{
		auto opcode = oc::CPX_ABS;

		bus.write(addressing_ABS(opcode), 0x02);
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

		bus.write(addressing_ZPG(opcode), 0x02);
		compare(opcode, cpu.Y);
	}

	TEST_F(cpu6502_test, CPY_ABS)
	{
		auto opcode = oc::CPY_ABS;

		bus.write(addressing_ABS(opcode), 0x02);
		compare(opcode, cpu.Y);
	}

	void cpu6502_test::increment(oc opcode, uint16_t addr)
	{
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x03);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, INC_ZPG)
	{
		auto opcode = oc::INC_ZPG;

		auto addr = addressing_ZPG(opcode);
		bus.write(addr, 0x02);
		increment(opcode, addr);
	}

	TEST_F(cpu6502_test, INC_ZPX)
	{
		auto opcode = oc::INC_ZPX;

		auto addr = addressing_ZP(opcode, cpu.X);
		bus.write(addr, 0x02);
		increment(opcode, addr);
	}

	TEST_F(cpu6502_test, INC_ABS)
	{
		auto opcode = oc::INC_ABS;

		auto addr = addressing_ABS(opcode);
		bus.write(addr, 0x02);
		increment(opcode, addr);
	}

	TEST_F(cpu6502_test, INC_ABX)
	{
		auto opcode = oc::INC_ABX;

		auto addr = addressing_AB(opcode, cpu.X);
		bus.write(addr, 0x02);
		increment(opcode, addr);
	}

	TEST_F(cpu6502_test, INC_ABX_C)
	{
		auto opcode = oc::INC_ABX;

		carry = true;
		auto addr = addressing_AB(opcode, cpu.X);
		bus.write(addr, 0x02);
		increment(opcode, addr);
	}

	TEST_F(cpu6502_test, INX)
	{
		auto opcode = oc::INX____;

		bus.write(address++, opcode);
		cpu.X = 0x02;
		cpu.execute();

		EXPECT_EQ(cpu.X, 0x03);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, INY)
	{
		auto opcode = oc::INY____;

		bus.write(address++, opcode);
		cpu.Y = 0x02;
		cpu.execute();

		EXPECT_EQ(cpu.Y, 0x03);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	void cpu6502_test::decrement(oc opcode, uint16_t addr)
	{
		cpu.execute();

		EXPECT_EQ(bus.read(addr), 0x01);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, DEC_ZPG)
	{
		auto opcode = oc::DEC_ZPG;

		auto addr = addressing_ZPG(opcode);
		bus.write(addr, 0x02);
		decrement(opcode, addr);
	}

	TEST_F(cpu6502_test, DEC_ZPX)
	{
		auto opcode = oc::DEC_ZPX;

		auto addr = addressing_ZP(opcode, cpu.X);
		bus.write(addr, 0x02);
		decrement(opcode, addr);
	}

	TEST_F(cpu6502_test, DEC_ABS)
	{
		auto opcode = oc::DEC_ABS;

		auto addr = addressing_ABS(opcode);
		bus.write(addr, 0x02);
		decrement(opcode, addr);
	}

	TEST_F(cpu6502_test, DEC_ABX)
	{
		auto opcode = oc::DEC_ABX;

		auto addr = addressing_AB(opcode, cpu.X);
		bus.write(addr, 0x02);
		decrement(opcode, addr);
	}

	TEST_F(cpu6502_test, DEC_ABX_C)
	{
		auto opcode = oc::DEC_ABX;

		carry = true;
		auto addr = addressing_AB(opcode, cpu.X);
		bus.write(addr, 0x02);
		decrement(opcode, addr);
	}

	TEST_F(cpu6502_test, DEX)
	{
		auto opcode = oc::DEX____;

		bus.write(address++, opcode);
		cpu.X = 0x02;
		cpu.execute();

		EXPECT_EQ(cpu.X, 0x01);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}

	TEST_F(cpu6502_test, DEY)
	{
		auto opcode = oc::DEY____;

		bus.write(address++, opcode);
		cpu.Y = 0x02;
		cpu.execute();

		EXPECT_EQ(cpu.Y, 0x01);
		EXPECT_EQ(clock.get_cycles(), cpu.instructions.at(opcode).cycles);
	}
}